﻿#ifndef _SYSTEM_UTILITY_H_
#define _SYSTEM_UTILITY_H_

#include "ThreadLock.h"
#include "FileUtility.h"

class SystemUtility : public FileUtility
{
	static ThreadLock mTimeLock;
public:
	static void stop();
	static void sleep(unsigned long timeMS);
	static long getTimeMS();
	static const char* getTime();
	// 返回media的路径,不带/
	static const string& getMediaPath()
	{
		return MEDIA_PATH;
	}
	// 通过一个media下的相对路径,或者绝对路径,转化为一个可直接打开的路径
	static string getAvailableResourcePath(string name)
	{
		string mediaPath = getMediaPath();
		// 如果文件名已经是不带media路径并且不是绝对路径
		if (mediaPath != "" && (name.length() <= mediaPath.length() || name.substr(0, mediaPath.length()) != mediaPath) && (name.length() > 1 && name[1] != ':'))
		{
			name = mediaPath + "/" + name;
		}
		return name;
	}

	static string getIPFromSocket(const TX_SOCKET& socket)
	{
#if RUN_PLATFORM == PLATFORM_WINDOWS
		SOCKADDR_IN addr_conn;
#elif RUN_PLATFORM == PLATFORM_LINUX
		struct sockaddr_in addr_conn;
#endif
		int nSize = sizeof(addr_conn);
		memset((void *)&addr_conn, 0, sizeof(addr_conn));
#if RUN_PLATFORM == PLATFORM_WINDOWS
		getpeername(socket, (SOCKADDR *)(&addr_conn), &nSize);
#elif RUN_PLATFORM == PLATFORM_LINUX
		getpeername(socket, (struct sockaddr *)(&addr_conn), (socklen_t*)&nSize);
#endif
		return string(inet_ntoa(addr_conn.sin_addr));
	}

	static void print(const string& str, bool nextLine = true)
	{
#if RUN_PLATFORM == PLATFORM_WINDOWS
		cout << str;
		if (nextLine)
		{
			cout << endl;
		}
#elif RUN_PLATFORM == PLATFORM_LINUX
		printf(str.c_str());
		if (nextLine)
		{
			printf("\n");
		}
#endif
	}
	static void input(string& str)
	{
#if RUN_PLATFORM == PLATFORM_WINDOWS
		cin >> str;
#elif RUN_PLATFORM == PLATFORM_LINUX
		scanf(str.c_str());
#endif
	}
	
	static int getThreadID()
	{
#if RUN_PLATFORM == PLATFORM_WINDOWS
		int threadID = GetCurrentThreadId();
#elif RUN_PLATFORM == PLATFORM_ANDROID
		int threadID = pthread_self();
#endif
		return threadID;
	}
};

#endif
