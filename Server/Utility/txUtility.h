#ifndef _TX_UTILITY_H_
#define _TX_UTILITY_H_

#include "txMemoryTrace.h"
#include "ServerDefine.h"

class txUtility
{
public:
	static void stop();
	static void logError(const std::string& info)
	{
#if RUN_PLATFORM == PLATFORM_WINDOWS
		std::cout << "程序错误 : " << info.c_str() << std::endl;
#elif RUN_PLATFORM == PLATFORM_LINUX
		printf("程序错误 : %s\n", info.c_str());
#endif
	}
	static void logInfo(const std::string& info)
	{
#if RUN_PLATFORM == PLATFORM_WINDOWS
		std::cout << info.c_str() << std::endl;
#elif RUN_PLATFORM == PLATFORM_LINUX
		printf("%s\n", info.c_str());
#endif
	}
	static void sleep(const unsigned long& timeMS);
	static unsigned long getTimeMS();
	static const char* getTime();
	// 返回media的路径,不带/
	static const std::string& getMediaPath()
	{
		return MEDIA_PATH;
	}
	// 通过一个media下的相对路径,或者绝对路径,转化为一个可直接打开的路径
	static std::string getAvailableResourcePath(std::string name)
	{
		std::string mediaPath = txUtility::getMediaPath();
		// 如果文件名已经是不带media路径并且不是绝对路径
		if (mediaPath != "" && (name.length() <= mediaPath.length() || name.substr(0, mediaPath.length()) != mediaPath) && (name.length() > 1 && name[1] != ':'))
		{
			name = mediaPath + "/" + name;
		}
		return name;
	}

	static std::string getIPFromSocket(const TX_SOCKET& socket)
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
		return std::string(inet_ntoa(addr_conn.sin_addr));
	}

	template<typename T>
	static void inverseByte(T& value)
	{
		int typeSize = sizeof(T);
		for (int i = 0; i < typeSize / 2; ++i)
		{
			swapByte(value, i, typeSize - i - 1);
		}
	}
	template<typename T>
	static void swapByte(T& value, const int& pos0, const int& pos1)
	{
		char byte0 = (value & (0xff << (8 * pos0))) >> (8 * pos0);
		char byte1 = (value & (0xff << (8 * pos1))) >> (8 * pos1);
		SET_BYTE(value, byte0, pos1);
		SET_BYTE(value, byte1, pos0);
	}

	// 秒数转换为分数和秒数
	static void secondsToMinutesSeconds(const int& seconds, int& outMin, int& outSec)
	{
		outMin = seconds / 60;
		outSec = seconds - outMin * 60;
	}

	static void secondsToHoursMinutesSeconds(const int& seconds, int& outHour, int& outMin, int& outSec)
	{
		outHour = seconds / (60 * 60);
		outMin = (seconds - outHour * (60 * 60)) / 60;
		outSec = seconds - outHour * (60 * 60) - outMin * 60;
	}
};

#endif
