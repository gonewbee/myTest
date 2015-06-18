/**
 * @brief 发送心跳包检测网络连接情况
 * 
 * 测试编译gcc -D_DEBUG_TEST_ heartbeat.cpp -lpthread
 */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

/**
 * @brief 建立tcp连接
 * @return 大于0成功，其它值失败
 */
static int tcp_connect(const char *hostname, int port) {
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
	status = connect(sockfd, addr->ai_addr, addr->ai_addrlen);
	if (status<0) {
		fprintf(stderr, "connect error\n");
		return -1;
	}
	return sockfd;
}

void *heartbeat_thread(void *arg) {
	int alive_num = 0;
	char str[] = "keep alive";
	char buf[64] = {0};
	int sockfd = *((int *)arg);
	int len = 0;
	fprintf(stderr, "in heartbeat_thread sockfd:%d %p\n", sockfd, arg);
	while (1) {
		if (send(sockfd, str, strlen(str), 0)==-1) {
			perror("send error");
			goto fail;
		}
		len = recv(sockfd, buf, 64, 0);
		if (len==-1) {
			perror("recv error");
			goto fail;
		} else if (len==0) {
			fprintf(stderr, "recv nothing\n");
			alive_num++;
			if (alive_num>=3) {
				fprintf(stderr, "time out\n");
				goto fail;
			}
		} else if (len>0) {
			alive_num = 0;
			fprintf(stderr, "recv:%s\n", buf);
			memset(buf, 0, 64);
		}
		sleep(3);
	}
fail:
	close(sockfd);
	exit(1);
}

/**
 * @brief 启动心跳检测
 *
 * 启动一个线程向目的地址发送心跳包
 * @param[in] hostname 目的主机ip地址
 * @param[in] port 目的主机端口
 */
int start_heartbeat(const char* hostname, int port) {
	int sockfd = -1;
	pthread_t pid = 0;
	int ret = -1;
	
	sockfd = tcp_connect(hostname, port);
	if (sockfd<0) {
		fprintf(stderr, "tcp connect error\n");
		return -1;
	}
	fprintf(stderr, "tcp_connect sockfd:%d %p\n", sockfd, &sockfd);
	
	ret = pthread_create(&pid, NULL, heartbeat_thread, (void *)(&sockfd));
	sleep(1); /* pthread_create传递参数时，如果start_heartbeat立刻结束的话，&sockfd对应的值已经释放，在heartbeat_thread获取到的arg就不对 */
	if (ret!=0) {
		fprintf(stderr, "pthread_create error\n");
		return -1;
	}
	return 1;
}

#ifdef _DEBUG_TEST_
int main(int argc, char **argv) {
	start_heartbeat("192.168.7.169", 12138);
	fprintf(stderr, "wait input!");
	getchar();
}
#endif
#ifdef __cplusplus
}
#endif
