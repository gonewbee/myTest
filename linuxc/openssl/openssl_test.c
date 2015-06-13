/**
 * 编译：gcc openssl_test.c -lssl -lcrypto
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <unistd.h>

void InitializeSSL()
{
    SSL_load_error_strings();
    SSL_library_init();
    OpenSSL_add_all_algorithms();
}

void DestroySSL()
{
    ERR_free_strings();
    EVP_cleanup();
}

void ShutdownSSL(SSL *ssl)
{
        SSL_shutdown(ssl);
        SSL_free(ssl);
}

/**
 * @brief 将socket设置位非阻塞模式
 */
int set_nonblocking_mode(int sockfd) {
	int flags;
	flags = fcntl(sockfd, F_GETFL);
	if (flags<0) {
		fprintf(stderr, "set_nonblocking_mode failed\n");
		return -1;
	}
	fcntl(sockfd, F_SETFL, flags|O_NONBLOCK);	/* 设置位非阻塞 */
	return 1;
}

/**
 * @brief 设置receive buffer
 */
int set_tcp_rcvbuf(int sockfd) {
	unsigned int optval;
	socklen_t optlen;
	
	optval = 1;
	optlen = sizeof(optval);
	/* 在套接字上禁用Nagle算法 (只适用于流式套接字) */
	if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (void*) &optval, optlen)<0) {
		fprintf(stderr, "unable to set TCP_NODELAY");
	}
	
	/* receive buffer must be a least 32 K */
	if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (void*) &optval, &optlen)==0) {
		fprintf(stdout, "SO_RCVBUF:%d\n", optval);
		if (optval<(1024*32)) {
			optval = 1024*32;
			optlen = sizeof(optval);
			
			if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (void*) &optval, optlen)<0) {
				fprintf(stderr, "unable to set receive buffer len");
				return -1;
			}
		}
	}
	return 1;
}

void tcp_set_keep_alive_mode(int sockfd) {
	unsigned int optval;
	socklen_t optlen;
	
	optval = 1;
	optlen = sizeof(optval);
	/* 启动keep alive */
	if (setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (void*) &optval, optlen)<0) {
		fprintf(stderr, "setsockopt() SOL_SOCKET, SO_KEEPALIVE error\n");
	}
	
	optval = 5;
	optlen = sizeof(optval);
	/* 设置空闲时间 */
	if (setsockopt(sockfd, SOL_TCP, TCP_KEEPIDLE, (void*) &optval, optlen)<0) {
		fprintf(stderr, "setsockopt() SOL_TCP, TCP_KEEPIDLE error\n");
	}
	
	optval = 3;
	optlen = sizeof(optval);
	/* 连接次数 */
	if (setsockopt(sockfd, SOL_TCP, TCP_KEEPCNT, (void*) &optval, optlen)<0) {
		fprintf(stderr, "setsockopt() SOL_TCP, TCP_KEEPCNT error\n");
	}
	
	optval = 2;
	optlen = sizeof(optval);
	/* 连接时间间隔 */
	if (setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (void*) &optval, optlen)<0) {
		fprintf(stderr, "setsockopt() SOL_TCP, TCP_KEEPINTVL error\n");
	}
}

int connect_timeout(int sockfd, struct sockaddr* addr, socklen_t addrlen, int timeout) {
#if 1
	fd_set cfds;
	struct timeval tv;
	int status;
	socklen_t optlen;
	
	set_nonblocking_mode(sockfd);
	/* 先设置为非阻塞模式再connect，如果connect不能马上完成就返回EINPROGRESS */
	status = connect(sockfd, addr, addrlen);
	if (status>=0)
		return 1;
	perror("connect");
	if (errno!=EINPROGRESS) {
		return -1;
	}
	FD_SET(sockfd, &cfds);
	tv.tv_sec = timeout;
	tv.tv_usec = 0;
	status = select(sockfd + 1, NULL, &cfds, NULL, &tv);
	if (status==0) {
		fprintf(stderr, "connect timeout\n");
		close(sockfd);
		return -1;
	}
	
	status = 0;
	optlen = sizeof(status);
	if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (void*) &status, &optlen) < 0) {
		return -1;
	}
	if (status!=0) {
		return -1;
	}
	return 1;
#endif	
}

/**
 * @brief 建立一个tcp连接
 * @param[in] hostname 服务端地址
 * @param[in] port 服务端端口
 * @return 成功返回socket值，失败返回-1
 */
int tcp_connect(const char* hostname, int port) {
	int status;
	int sockfd;
	char port_str[16];
	struct addrinfo hints;
	struct addrinfo* addr;
	struct addrinfo* result;
	
	/* 初始化hints */
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	/* 将port由int转化为字符串 */
	snprintf(port_str, sizeof(port_str)-1, "%u", port);
	fprintf(stderr, "hostname:%s;port_str:%s\n", hostname, port_str);
	
	status = getaddrinfo(hostname, port_str, &hints, &result);
	if (status!=0) {
		fprintf(stderr, "getaddrinfo error:%s\n", gai_strerror(status));
		return -1;
	}
	
	addr = result;
	if ((addr->ai_family==AF_INET6) && (addr->ai_next!=0)) {
		while ((addr==addr->ai_next)) {
			if (addr->ai_family==AF_INET)
				break;
		}
		if (!addr) {
			addr = result;
		}
	}
	
	sockfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	if (sockfd<0) {
		perror("socket");
		freeaddrinfo(result); /* 释放由getaddrinfo接口返回指针 */
		return -1;
	}

	if (connect_timeout(sockfd, addr->ai_addr, addr->ai_addrlen, 2)<0) {
		fprintf(stderr, "connect_timeout error\n");
		freeaddrinfo(result);
		return -1;
	}
	freeaddrinfo(result);
	fprintf(stderr, "success tcp_connect");
	set_tcp_rcvbuf(sockfd);
	tcp_set_keep_alive_mode(sockfd);
	return sockfd;
}

int main(int argc, char *argv) {
	SSL *ssl = NULL;
	SSL_CTX *sslctx = NULL;
	X509 *server_cert = NULL;
	char buffer[256] = {0};
	int sockfd = -1;
	int options = 0;
	int connection_status;
	int status;
	int error;
	
	sockfd = tcp_connect("192.168.1.72", 3389);
	fprintf(stderr, "sockfd:%d\n", sockfd);
	if (sockfd<0) {
		return -1;
	}
	
	InitializeSSL();
	
	/* 设置为客户端tls */
	sslctx = SSL_CTX_new(TLSv1_client_method());
	if (!sslctx) {
		fprintf(stderr, "SSL_CTX_new failed\n");
		return -1;
	}
	
	fprintf(stderr, "SSL_ERROR_WANT_READ:%d;SSL_ERROR_WANT_WRITE:%d\n", SSL_ERROR_WANT_READ,SSL_ERROR_WANT_WRITE);
	
	options |= SSL_OP_TLS_BLOCK_PADDING_BUG;
	options |= SSL_OP_DONT_INSERT_EMPTY_FRAGMENTS;
	SSL_CTX_set_options(sslctx, options);
	
	ssl = SSL_new(sslctx);
	if (!ssl) {
		fprintf(stderr, "SSL_new failed\n");
		return -1;
	}
	
	if (SSL_set_fd(ssl, sockfd)<1) {
		fprintf(stderr, "SSL_set_fd failed\n");
		return -1;
	}
	//建立SSL链接
	connection_status = SSL_connect(ssl);
	fprintf(stderr, "connection_status:%d\n", connection_status);
	if (connection_status<=0) {
		error = SSL_get_error(ssl, connection_status);
		fprintf(stderr, "SSL_connect connection_status:%d;error:%d\n", connection_status, error);
		if ((error!=SSL_ERROR_WANT_READ)&&(error!=SSL_ERROR_WANT_WRITE)) {
			close(sockfd);
			return -1;
		}
	}
	//获取服务器端证书
	server_cert = SSL_get_peer_certificate(ssl);
	//释放服务器端证书
	X509_free(server_cert);
	while (1) {
		status = SSL_read(ssl, buffer, 255);
		fprintf(stderr, "SSL_read status:%d\n", status);
		if (status==0) {
			fprintf(stderr, "disconnect\n");
			close(sockfd);
			return -1;
		}
		if (status<=0) {
			error = SSL_get_error(ssl, status);
			fprintf(stderr, "SSL_read error:%d\n", error);
		}
		sleep(1);
	}
	
	ShutdownSSL(ssl);
	close(sockfd);
	SSL_CTX_free(sslctx);
	return 0;
}
