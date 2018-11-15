#ifndef _SERVER_ENUM_H_
#define _SERVER_ENUM_H_

// 服务器配置文件浮点数参数定义
enum FRAME_DEFINE_FLOAT
{
	FDF_HEART_BEAT_TIME_OUT,	// 心跳超时时间
	FDF_SOCKET_PORT,			// socket端口号
	FDF_BACK_LOG,				// 连接请求队列的最大长度
	FDF_SHOW_COMMAND_DEBUG_INFO,// 是否显示命令调试信息
	FDF_OUTPUT_NET_LOG,			// 是否显示网络日志信息
	FDF_MAX,
};
// 服务器配置文件字符串参数定义
enum FRAME_DEFINE_STRING
{
	FDS_DOMAIN_NAME,			// 连接的服务器域名
	FDS_MAX,
};

enum PARSE_RESULT
{
	PR_SUCCESS,
	PR_NOT_ENOUGH,
	PR_ERROR,
};

#endif