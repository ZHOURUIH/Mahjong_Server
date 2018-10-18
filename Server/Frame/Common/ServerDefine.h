#ifndef _SERVER_DEFINE_H_
#define _SERVER_DEFINE_H_

// ƽ̨��ʶ��
#define PLATFORM_WINDOWS 0
#define PLATFORM_LINUX 1
#define PLATFORM_ANDROID PLATFORM_LINUX

// �������е�ƽ̨��ʶ
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
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#pragma warning(disable: 4005)
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
#include <atomic>

#include "ServerEnum.h"
#include "ServerCallback.h"

//-----------------------------------------------------------------------------------------------------------------------------------------------
// �궨��
#if RUN_PLATFORM == PLATFORM_WINDOWS
#define _USE_SAFE_API
#define TX_THREAD HANDLE
#define TX_SOCKET SOCKET
#define NULL_THREAD NULL
#define THREAD_CALLBACK_DECLEAR(func) static DWORD WINAPI func(LPVOID args)
#define THREAD_CALLBACK(class, func) DWORD WINAPI class##::##func(LPVOID args)
#define CREATE_THREAD(thread, func, args) thread = CreateThread(NULL, 0, func, args, 0, NULL)
#define CLOSE_THREAD(thread)	\
if (thread != NULL_THREAD)		\
{								\
	TerminateThread(thread, 0);	\
	CloseHandle(thread);		\
	thread = NULL_THREAD;		\
}
#define CLOSE_SOCKET(socket) closesocket(socket);
#elif RUN_PLATFORM == PLATFORM_LINUX
#define TX_THREAD pthread_t
#define TX_SOCKET unsigned int
#define NULL_THREAD 0
#define SOCKADDR_IN sockaddr_in
#define THREAD_CALLBACK_DECLEAR(func) static void* func(void* args)
#define THREAD_CALLBACK(class, func) void* class##::##func(void* args)
#define CREATE_THREAD(thread, func, args) pthread_create(&thread, NULL, func, args);
#define CLOSE_THREAD(thread)	\
if (thread != NULL_THREAD)		\
{								\
	pthread_cancel(thread);		\
	thread = NULL_THREAD;		\
}
#define CLOSE_SOCKET(socket) close(socket);
#ifdef __GNUC__
#define CSET_GBK    "GBK"
#define CSET_UTF8   "UTF-8"
#define LC_NAME_zh_CN   "zh_CN"
#endif
#define LC_NAME_zh_CN_GBK       LC_NAME_zh_CN "." CSET_GBK
#define LC_NAME_zh_CN_UTF8      LC_NAME_zh_CN "." CSET_UTF8
#define LC_NAME_zh_CN_DEFAULT   LC_NAME_zh_CN_GBK
#endif

#ifdef _USE_SAFE_API
#define SPRINTF(buffer, bufferSize, ...) sprintf_s(buffer, bufferSize, __VA_ARGS__)
#else
#define SPRINTF(buffer, bufferSize, ...) sprintf(buffer, __VA_ARGS__)
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (unsigned int)~0
#endif

#define INVALID_ID (unsigned int)~0
#define INVALID_INT_ID -1

#ifndef NULL
#define NULL 0
#endif

// �ٴη�װ��������ı�����
#define FOR(stl, expression)stl.lock(SL_WRITE, __FILE__, __LINE__);for (expression)
#define END(stl) stl.unlock(SL_WRITE);
#define TOSTRING(t) #t
#define LINE_STR(v) TOSTRING(v)
// ����value��ָ��λ��pos���ֽڵ�ֵΪbyte,���Ҳ�Ӱ�������ֽ�
#define SET_BYTE(value, byte, pos) value = (value & ~(0xFF << (8 * pos))) | (byte << (8 * pos))
// ���value��ָ��λ��pos���ֽڵ�ֵ
#define GET_BYTE(value, pos) (value & (0xFF << (8 * pos))) >> (8 * pos)
#define _FILE_LINE_ "File : " + std::string(__FILE__) + ", Line : " + LINE_STR(__LINE__)
#define NEW_PACKET(packet, type) NetServer::createPacket(packet, type);

// ��ɫΨһID
typedef unsigned int CHAR_GUID;
// ÿ���ͻ��˵�ΨһID
typedef unsigned int CLIENT_GUID;

// ��󲢷�������Ϊ64
#ifdef FD_SETSIZE
#undef FD_SETSIZE
#define FD_SETSIZE 64
#endif

#define LOCK(l) \
(l).waitForUnlock(__FILE__, __LINE__);\
try\
{

#define UNLOCK(l) \
}catch(...){}\
(l).unlock()

#include "GameLogWrap.h"
#include "txVector.h"
#include "txMap.h"
#include "txSet.h"
#include "Vector2.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// �ṹ�嶨��

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// �������ֶ���
const int CLIENT_TEMP_BUFFER_SIZE = 11 * 1024;	// �ͻ�����ʱ��������С,Ӧ�ò�С�ڵ�����Ϣ�����Ĵ�С
const int CLIENT_SEND_BUFFER_SIZE = 8 * 1024 * 1024;// �ͻ��˷������ݻ�������С,8MB
const int CLIENT_RECV_BUFFER_SIZE = 8 * 1024 * 1024;// �ͻ��˽������ݻ�������С,8MB
const int SERVER_RECV_BUFFER_SIZE = 8 * 1024 * 1024;	// ��socket��������ʱʹ�õĻ�����,8MB
const int HEADER_SIZE = sizeof(int) + sizeof(int);

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// �����ַ�������
const std::string MEDIA_PATH = "../media";
const std::string GAME_DATA_PATH = "GameDataFile/";
const std::string CONFIG_PATH = "Config/";
const std::string LOG_PATH = "Log/";
const std::string EMPTY_STRING = "";

#endif