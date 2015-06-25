#ifndef HEARTBEAT_H
#define HEARTBEAT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef int (*HEARTBEAT_DIS)(int);

typedef struct heartbeat_arg {
	int sockfd;	/**< socket句柄 */
	char hostname[16];	/**< 服务端ip */
	int port;	/**< 服务端口 */
	int interval;	/**< 心跳时间间隔 */
	int num;	/**< 断开连接的判断次数 */
	HEARTBEAT_DIS disconnect;	/**< 断开连接的处理函数 */
} _p_heartbeat_arg;

/**
 * @brief 申请一个heartbeat
 */
_p_heartbeat_arg *new_heartbeat();

void free_heartbeat(_p_heartbeat_arg *heartbeat);

int start_heartbeat(_p_heartbeat_arg *heartbeat);

int stop_heartbeat(_p_heartbeat_arg *heartbeat);

#ifdef __cplusplus
}
#endif
#endif // HEARTBEAT_H
