#ifndef _SERVER_DEFINE_H_
#define _SERVER_DEFINE_H_

// 平台标识宏
#define PLATFORM_WINDOWS 0
#define PLATFORM_LINUX 1

// 正在运行的平台标识
#ifdef WINDOWS
#define RUN_PLATFORM PLATFORM_WINDOWS
#endif

#ifdef LINUX
#define RUN_PLATFORM PLATFORM_LINUX
#endif

#ifndef RUN_PLATFORM
#define RUN_PLATFORM -1
#error "wrong platform!"
#endif

#if RUN_PLATFORM == PLATFORM_WINDOWS
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <io.h>
#include <direct.h>
#include <winsock.h>
#endif
#if RUN_PLATFORM == PLATFORM_LINUX
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/un.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdarg.h>
#include <signal.h>
#include <dirent.h>
#include <pthread.h>
#endif
#include <string>
#include <map>
#include <vector>
#include <set>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <typeinfo>
#include <memory>
#include <time.h>
#include <cmath>
#include <math.h>
#include <sys/types.h>
#include <cctype>
#include <algorithm>
#include <assert.h>
#include <mysql.h>

#if RUN_PLATFORM == PLATFORM_LINUX
#define SOCKET unsigned int
#define SOCKADDR_IN sockaddr_in
#endif

#if RUN_PLATFORM == PLATFORM_WINDOWS
#define _USE_SAFE_API
#endif

#if RUN_PLATFORM == PLATFORM_WINDOWS
#define TX_THREAD HANDLE
#define TX_SOCKET SOCKET
#define NULL_THREAD NULL
#elif RUN_PLATFORM == PLATFORM_LINUX
#define TX_THREAD pthread_t
#define TX_SOCKET unsigned int
#define NULL_THREAD 0
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET ~0
#endif

#ifndef NULL
#define NULL 0
#endif

const std::string MEDIA_PATH = "../media";
const std::string GAME_DATA_PATH = "GameDataFile/";
const std::string CONFIG_PATH = "Config/";
const std::string EMPTY_STRING = "";

#define TX_MAX(x, y) ((x) > (y) ? (x) : (y))
#define TX_MIN(x, y) ((x) < (y) ? (x) : (y))

//角度转弧度
#define AngleToRadian(angle) angle * 3.141592654 / 180.0

//弧度转角度
#define RadianToAngle(radian) radian * 180.0 / 3.141592654

#define TOSTRING(t) #t

// 设置value的指定位置pos的字节的值为byte,并且不影响其他字节
#define SET_BYTE(value, byte, pos) value = (value & ~(0x000000ff << (8 * pos))) | (byte << (8 * pos))
// 获得value的指定位置pos的字节的值
#define GET_BYTE(value, pos) (value & (0x000000ff << (8 * pos))) >> (8 * pos)

enum SERVER_DEFINE
{
	SD_HEART_BEAT_TIME_OUT,		// 心跳超时时间
	SD_SOCKET_PORT,				// socket端口号
	SD_BACK_LOG,				// 连接请求队列的最大长度
	SD_SHOW_COMMAND_DEBUG_INFO,	// 是否显示命令调试信息
	SD_OUTPUT_NET_LOG,			// 是否显示网络日志信息
	SD_MAX,
};

#ifdef _USE_SAFE_API
#define SPRINTF(buffer, bufferSize, ...) sprintf_s(buffer, bufferSize, __VA_ARGS__)
#else
#define SPRINTF(buffer, bufferSize, ...) sprintf(buffer, __VA_ARGS__)
#endif

#define _FILE_LINE_ "file : " + txUtility::getFileName(__FILE__) + ", line : " + txUtility::intToString(__LINE__)

// 角色唯一ID
typedef unsigned long CHAR_GUID;
// 每个客户端的唯一ID
typedef unsigned long CLIENT_GUID;

// 最大并发连接数为64
#ifdef FD_SETSIZE
#undef FD_SETSIZE
#define FD_SETSIZE 64
#endif

const int HEADER_SIZE = sizeof(short)+sizeof(short);

// 线程加锁类型
enum LOCK_TYPE
{
	LT_READ,		// 锁定后需要进行读取
	LT_WRITE,		// 锁定后需要进行写入
};
#define LOCK(l, flag) l.waitForUnlock(__FILE__, __LINE__, flag)
#define UNLOCK(l, flag) l.unlock(flag)

struct FINISH_DATA
{
	CHAR_GUID mPlayerGUID;	// 玩家ID
	float mCalories;		// 卡路里
	float mBestTime;		// 单圈最佳时间
	float mTotalTime;		// 骑行总时间
	float mHighSpeed;		// 最高速度
	float mAvgSpeed;		// 平均速度
	float mPercent;			// 当前路线的完成百分比
	int mRouteIndex;		// 当前路线的下标
};

class CharacterPlayer;
struct ROOM_PLAYER
{
	CharacterPlayer* mPlayer;
	bool mReady;
	bool mContinueFlag;
	ROOM_PLAYER()
	{
		mPlayer = NULL;
		mReady = false;
		mContinueFlag = false;
	}
	ROOM_PLAYER(CharacterPlayer* player, const bool& ready, const bool& continueFlag)
	{
		mPlayer = player;
		mReady = ready;
		mContinueFlag = continueFlag;
	}
};

const int MAX_PLAYER = 2;						// 房间中最大的玩家数量
const int MAP_MAX_COUNT = 3;					// 地图最大数量
const int MAX_LOAD_FILE_COUNT = 1024;			// 内存中同时存在的加载的文件的数量
const int CLIENT_TEMP_BUFFER_SIZE = 2 * 1024;	// 客户端临时缓冲区大小,应该不小于单个消息包最大的大小
const int CLIENT_BUFFER_SIZE = 512 * 1024;	// 客户端发送和接收数据缓冲区大小

#endif
