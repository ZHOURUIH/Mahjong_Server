#ifndef _TX_UTILITY_H_
#define _TX_UTILITY_H_

#include "txMemeryTrace.h"
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
	static const char* getTime();
#if RUN_PLATFORM == PLATFORM_LINUX
	static bool isDirectory(const char* pszName);
#endif
	static void findFiles(const char * pathName, txVector<std::string>& files, const txVector<std::string>& patterns, const bool& recursive = true);
	static void deleteFolder(const std::string& path);
	static bool isFileExist(const std::string& fullPath);
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

	static std::string getIPFromSocket(const SOCKET& socket)
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

	static std::string removeSuffix(const std::string& str)
	{
		int dotPos = str.find_last_of('.');
		if (dotPos != -1)
		{
			return str.substr(0, dotPos);
		}
		return str;
	}

	// 去掉最后一个逗号
	static void removeLastComma(std::string& stream)
	{
		int streamSize = stream.length();
		for (int i = 0; i < streamSize; ++i)
		{
			if (stream[streamSize - i - 1] == ',')
			{
				stream.erase(streamSize - i - 1, 1);
				break;
			}
		}
	}

	static std::string getFileName(std::string str)
	{
		rightToLeft(str);
		int dotPos = str.find_last_of('/');
		if (dotPos != -1)
		{
			return str.substr(dotPos + 1, str.length() - 1);
		}
		return str;
	}

	static std::string getFileNameNoSuffix(std::string str)
	{
		rightToLeft(str);
		int namePos = str.find_last_of('/');
		int dotPos = str.find_last_of('.');
		if (dotPos != -1)
		{
			return str.substr(namePos + 1, dotPos - namePos - 1);
		}
		return str;
	}
	static std::string getParentDir(std::string dir)
	{
		rightToLeft(dir);
		int pos = dir.find_last_of('/');
		std::string tempDir = dir;
		if (-1 != pos)
		{
			tempDir = dir.substr(0, pos);
		}
		return tempDir;
	}

	static std::string getFileSuffix(const std::string& fileName)
	{
		int dotPos = fileName.find_last_of('.');
		if (dotPos != -1)
		{
			return fileName.substr(dotPos + 1, fileName.length() - dotPos - 1);
		}
		return fileName;
	}

	static void split(std::string str, const char* deli, txVector<std::string>* vec);

	static std::string boolToString(const bool& value) { return value ? "True" : "False"; }
	static bool stringToBool(const std::string& str) { return str == "True" || str == "true"; }

	static std::string strReplace(const std::string& str, const int& begin, const int& end, const std::string& reStr);
	static float calculateFloat(std::string str);	// 以浮点数的计算法则计算一个表达式,只支持加减乘除和括号
	static int calculateInt(std::string str);		// 以整数的计算法则计算一个表达式,支持取余,加减乘除和括号
	static std::string intToString(const int& i, const int& limitLen = 0); // limitLen是字符串的最小长度,如果整数的位数不足最小长度,则会在前面加0
	static int stringToInt(const std::string& str)
	{
		return atoi(str.c_str());
	}
	static float pow_f(const float& f, int p);
	static std::string floatToString(float f, const int& precision);	//precision -> 精度,保留的小数的位数
	static float stringToFloat(const std::string& str)
	{
		return (float)atof(str.c_str());
	}

	static float randomFloat(const float& minFloat, const float& maxFloat)
	{
		float percent = (rand() % (1000 + 1)) / 1000.0f;
		return percent * (maxFloat - minFloat) + minFloat;
	}
	static int randomInt(const int& minInt, const int& maxInt)
	{
		if (minInt >= maxInt)
		{
			return minInt;
		}
		return rand() % (maxInt - minInt + 1) + minInt;
	}

	// 判断oriString是否以pattern结尾
	static bool endWith(const std::string& oriString, const std::string& pattern)
	{
		if (oriString.length() < pattern.length())
		{
			return false;
		}
		return oriString.substr(oriString.length() - pattern.length(), pattern.length()) == pattern;
	}
	// 判断oriString是否以pattern开头
	static bool startWith(const std::string& oriString, const std::string& pattern)
	{
		if (oriString.length() < pattern.length())
		{
			return false;
		}
		std::string startString = oriString.substr(0, pattern.length());
		return startString == pattern;
	}

	// 计算 16进制的c中1的个数
	static int crc_check(const char& c)
	{
		int count = 0;
		int bitCount = sizeof(char) * 8;
		for (int i = 0; i < bitCount; ++i)
		{
			if ((c & (0x01 << i)) > 0)
			{
				++count;
			}
		}
		return count;
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

#if RUN_PLATFORM == PLATFORM_WINDOWS
	static std::wstring ANSIToUnicode(const std::string& str);
	static std::string UnicodeToANSI(const std::wstring& str);
	static std::string UnicodeToUTF8(const std::wstring& str);
	static std::wstring UTF8ToUnicode(const std::string& str);
	static std::string ANSIToUTF8(const std::string& str);
	static std::string UTF8ToANSI(const std::string& str);
#endif

	// 将sourceFile拷贝到destFile,sourceFile和destFile都是带可直接访问的路径的文件名,overWrite指定当目标文件已经存在时是否要覆盖文件
	static bool copyFile(const std::string& sourceFile, const std::string& destFile, bool overWrite = true);
	// 创建一个文件夹,path是一个不以/结尾的可直接访问的相对或者绝对的文件夹名
	static bool createFolder(const std::string& path);
	static bool writeFile(std::string filePath, const int& length, const char* buffer);
	static void rightToLeft(std::string& str)
	{
		int pathLength = str.length();
		for (int i = 0; i < pathLength; ++i)
		{
			if ('\\' == str[i])
			{
				str[i] = '/';
			}
		}
	}

	static int strFind(const std::string& res, const std::string& sub, const int& startPose, const bool& fromStart);
	static bool findSubstr(std::string res, std::string dst, const bool& sensitive, int* pos = NULL, const int& startPose = 0, const bool& firstOrLast = true);
	
	template<typename T>
	static txVector<std::string> findSubstr(txMap<std::string, T>& res, const std::string& dst, const bool& sensitive)
	{
		txVector <std::string> retList;
		// 循环遍历,如果匹配到放入列表
		typename txMap<std::string, T>::const_iterator itr = res.begin();
		typename txMap<std::string, T>::const_iterator itrEnd = res.end();
		FOR_STL(itr, ; itr != itrEnd; ++itr)
		{
			const std::string& name = itr->first;
			if (txUtility::findSubstr(name, dst, sensitive))
			{
				retList.push_back(name);
			}
		}
		END_FOR_STL(itr);
		return retList;
	}

	static txVector<std::string> findSubstr(txVector<std::string>& res, const std::string& dst, const bool& sensitive)
	{
		txVector <std::string> retList;
		int listSize = res.size();
		FOR_STL(res, int i = 0; i < listSize; ++i)
		{
			if (txUtility::findSubstr(res[i], dst, sensitive))
			{
				retList.push_back(res[i]);
			}
		}
		END_FOR_STL(res);
		return retList;
	}
	static void strToLower(std::string& str);
	static void strToUpper(std::string& str);

	// json
	static void jsonStartArray(std::string& str){ str += "["; }
	static void jsonEndArray(std::string& str)
	{ 
		removeLastComma(str);
		str += "],"; 
	}
	static void jsonStartStruct(std::string& str){ str += "{"; }
	static void jsonEndStruct(std::string& str)
	{
		removeLastComma(str);
		str += "},";
	}
	static void jsonAddPair(std::string& str, const std::string& name, const std::string& value)
	{
		str += "\"";
		str += name;
		str += "\":";
		str += "\"";
		str += value;
		str += "\",";
	}
};

#endif
