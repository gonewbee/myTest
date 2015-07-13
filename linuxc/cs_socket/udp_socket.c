/**
 * @brief udp socket的客户端和服务端程序
 *
 * 使用pkill -0 可以判断一个程序是否已运行
 * 使用pkexec相当于调用sudo
 */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <arpa/inet.h>

/**
 * @判断命令是否运行
 *
 * $ pkill -0 xfreerdp # -0 不是实际杀死应用，只是判断应用是否在运行
 * $ echo $? #查看上一条指令结果
 *  1 xfreerdp没有运行；；0 xfreerdp运行中
 */
int is_running() {
    int status = 1;
    status = system("pkill -0 xfreerdp");
    return status;
}

/**
 * @brief 使用pkexec相当于调用sudo，会弹出密码输入窗口
 */
void run_su_cmd() {
    int status = 1;
    status = system("pkexec ls /root");
    fprintf(stdout, "pkexec::%d\n", status);
}

/**
 * @brief 启动一个udp的客户端，向服务端发送信息
 */
int start_client() {
    int c_fd = -1;
    int port = -1;
    struct sockaddr_in s_addr;
    socklen_t slen;
    bzero(&s_addr,sizeof(s_addr));
    FILE *fp = fopen("setting.txt", "r");
    if (!fp) {
        perror("fopen");
        return -1;
    }
    fscanf(fp, "port:[%d]\n", &port); //从文件中读取服务端使用的端口
    fclose(fp);
    s_addr.sin_family=AF_INET;
    s_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    s_addr.sin_port=htons(port);
    slen = sizeof(s_addr);

    c_fd = socket(AF_INET,SOCK_DGRAM,0);//IPV4  SOCK_DGRAM 数据报套接字（UDP协议）
    struct timeval timeout={4,0};
    setsockopt(c_fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval)); //设置接收超时时间，需要使用struct timeval
    int i = 0;
    char buf[32] = {0};
    char recv[32] = {0};
    for (i=0; i<10; i++) {
        snprintf(buf, sizeof(buf)-1, "Hello World!::%u", i);
        if (-1==sendto(c_fd, buf, sizeof(buf), 0, (struct sockaddr *)&s_addr, slen)) {
            perror("sendto");
            break;        
        }
        fprintf(stdout, "send ok\n");
        if (-1==recvfrom(c_fd, recv, 32, 0, (struct sockaddr *)&s_addr, &slen)) {
            perror("recvfrom");
            break;
        }
        fprintf(stdout, "recv:%s\n", recv);
        sleep(1);
    }
    close(c_fd);
    return 0;
}

/**
 * @breif bind一个未使用的端口并将端口保存到文件中
 */
int start_server() {
    int s_fd = -1;
    int port = -1;

    struct sockaddr_in s_addr;//处理网络通信的地址
    struct sockaddr_in c_addr;
    bzero(&s_addr,sizeof(s_addr));
    s_addr.sin_family=AF_INET;
    s_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    s_addr.sin_port=htons(0); //bind一个未使用的端口
    
    s_fd =socket(AF_INET,SOCK_DGRAM,0);//IPV4  SOCK_DGRAM 数据报套接字（UDP协议）
    if(bind(s_fd, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1) {
        perror("bind");
        return -1;
    }
    socklen_t slen;
    slen = sizeof(s_addr);
    getsockname(s_fd, (struct sockaddr *)&s_addr, &slen); //获取使用的端口
    port = ntohs(s_addr.sin_port);
    fprintf(stdout, "port:%d\n", port);
    FILE *fp = fopen("setting.txt", "w");
    fprintf(fp, "port:[%d]\n", port);
    fclose(fp);

    socklen_t c_addr_len = (socklen_t)sizeof(c_addr);
    char message[32] = {0};
    char buf[] = "Response!!";
    while (1) {
        recvfrom(s_fd, message, 32, 0, (struct sockaddr *)&c_addr, &c_addr_len);
        sendto(s_fd, buf, sizeof(buf), 0, (struct sockaddr *)&c_addr, c_addr_len);
        fprintf(stdout, "%d message:%s\n", ntohs(c_addr.sin_port), message);
    }
    close(s_fd);
    return 0;
}

#ifdef _CLIENT
int main(int argc, char *argv[]) {
    int status = is_running();
    fprintf(stdout, "is running:%d\n", status);
    start_client();
}
#endif

#ifdef _SERVER
int main(int argc, char *argv[]) {
    run_su_cmd();
    start_server();
}
#endif

#ifdef __cplusplus
}
#endif
