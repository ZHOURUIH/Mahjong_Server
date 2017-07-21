﻿#include "txUtility.h"

#include "GameLog.h"
#include "ServerFramework.h"

void txUtility::stop()
{
	ServerFramework::getSingletonPtr()->stop();
}

const char* txUtility::getTime()
{
	static char timeBuffer[64] = {0};
#if RUN_PLATFORM == PLATFORM_WINDOWS
	static SYSTEMTIME sys;
	GetLocalTime(&sys);
	SPRINTF(timeBuffer, 64, "%d时%d分%d秒%d毫秒", sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
#elif RUN_PLATFORM == PLATFORM_LINUX
	static struct timeval tv;
	gettimeofday(&tv, NULL);
	unsigned long curMiliTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	const unsigned long& startMiliTime = ServerFramework::getSingletonPtr()->getStartMiliTime();
	unsigned long deltaMiliTime = curMiliTime - startMiliTime;
	unsigned long curSec = deltaMiliTime / 1000;
	unsigned long curMili = deltaMiliTime % 1000;
	SPRINTF(timeBuffer, 64, "%d秒%d毫秒", (int)curSec, (int)curMili);
#endif
	return timeBuffer;
}

#if RUN_PLATFORM == PLATFORM_LINUX
bool txUtility::isDirectory(const char* pszName)
{
	struct stat st;
	if(lstat(pszName, &st) < 0)
	{
		return false;
	}
	return S_ISDIR(st.st_mode);
}

void txUtility::findFiles(const char * pathName, txVector<std::string>& files, const txVector<std::string>& patterns, const bool& recursive)
{
	struct dirent *pDirent;
	DIR* pDir = opendir(pathName);
	if (pDir == NULL)
	{
		return;
	}
	while ((pDirent = readdir(pDir)) != NULL)
	{
		//如果是.或者..跳过
		if (std::string(pDirent->d_name) == "." || std::string(pDirent->d_name) == "..")
		{
			continue;
		}
		//判断是否为文件夹
		char szTmpPath[1024];
		memset(szTmpPath, 0, 1024);
		SPRINTF(szTmpPath, 1024, "%s/%s", pathName, pDirent->d_name);
		if (isDirectory(szTmpPath))
		{
			if(recursive)
			{
				//如果是文件夹则进行递归
				findFiles(szTmpPath, files, patterns, recursive);
			}
		}
		else
		{
			std::string fullName = std::string(szTmpPath);
			int patternCount = patterns.size();
			for (int i = 0; i < patternCount; ++i)
			{
				if (fullName.length() >= patterns[i].length() && fullName.substr(fullName.length() - patterns[i].length(), patterns[i].length()) == patterns[i])
				{
					files.push_back(fullName);
				}
			}
		}
	}
	closedir(pDir);
}

void txUtility::deleteFolder(const std::string& path)
{
	;
}

#endif

#if RUN_PLATFORM == PLATFORM_WINDOWS
void txUtility::findFiles(const char * pathName, txVector<std::string>& files, const txVector<std::string>& patterns, const bool& recursive)
{
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	std::string path(pathName);
	if (path.length() == 0)
	{
		return;
	}
	// 不以/结尾,则加上/
	if (path[path.length() - 1] != '/')
	{
		path += "/";
	}

	hFind = FindFirstFileA((path + "*").c_str(), &FindFileData);
	// 如果找不到文件夹就直接返回
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}
	while (::FindNextFileA(hFind, &FindFileData))
	{
		// 过滤.和..
		if (strcmp(FindFileData.cFileName, ".") == 0
			|| strcmp(FindFileData.cFileName, "..") == 0)
		{
			continue;
		}

		// 构造完整路径
		std::string fullname = path + std::string(FindFileData.cFileName);
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (recursive)
			{
				findFiles(fullname.c_str(), files, patterns, recursive);
			}
		}
		else
		{
			int patternCount = patterns.size();
			if (patternCount > 0)
			{
				for (int i = 0; i < patternCount; ++i)
				{
					if (fullname.length() >= patterns[i].length() && fullname.substr(fullname.length() - patterns[i].length(), patterns[i].length()) == patterns[i])
					{
						files.push_back(fullname);
					}
				}
			}
			else
			{
				files.push_back(fullname);
			}
		}
	}
	::FindClose(hFind);
}

void txUtility::deleteFolder(const std::string& path)
{
	WIN32_FIND_DATAA FindData;
	// 构造路径
	std::string pathName = path + "/*.*";
	HANDLE hFind = FindFirstFileA(pathName.c_str(), &FindData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}
	while (::FindNextFileA(hFind, &FindData))
	{
		// 过虑.和..
		if (strcmp(FindData.cFileName, ".") == 0
			|| strcmp(FindData.cFileName, "..") == 0)
		{
			continue;
		}

		// 构造完整路径
		std::string fullname = path + "/" + std::string(FindData.cFileName);
		// 如果是文件夹,则递归删除文件夹
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			deleteFolder(fullname);
		}
		// 如果是文件,则直接删除文件
		else
		{
			DeleteFileA(fullname.c_str());
		}
	}
	::FindClose(hFind);
	// 删除文件夹自身
	RemoveDirectoryA(path.c_str());
}
#endif

bool txUtility::isFileExist(const std::string& fullPath)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	int ret = _access(fullPath.c_str(), 0);
#elif RUN_PLATFORM == PLATFORM_LINUX
	int ret = access(fullPath.c_str(), 0);
#endif
	return ret == 0;
}

void txUtility::split(std::string str, const char* deli, txVector<std::string>* vec)
{
	while (true)
	{
		int devidePos = str.find_first_of(deli);
		if (devidePos == -1)
		{
			vec->push_back(str);
			break;
		}
		else
		{
			std::string curString = str.substr(0, devidePos);
			vec->push_back(curString);
			str = str.substr(devidePos + strlen(deli), str.length() - devidePos - strlen(deli));
		}
	}
}

std::string txUtility::strReplace(const std::string& str, const int& begin, const int& end, const std::string& reStr)
{
	std::string sub1 = str.substr(0, begin);
	std::string sub2 = str.substr(end, str.length() - end);
	return sub1 + reStr + sub2;
}

float txUtility::calculateFloat(std::string str)
{
	// 判断字符串是否含有非法字符,也就是除数字,小数点,运算符以外的字符
	for (int i = 0; i < (int)str.length();)
	{
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.' && str[i] != '+'
			&& str[i] != '-' && str[i] != '*' && str[i] != '/' && str[i] != '(' && str[i] != ')')
		{
			str.erase(i, 1);
		}
		else
		{
			++i;
		}
	}
	// 判断左右括号数量是否相等
	int leftBracketCount = 0;
	int rightBracketCount = 0;
	for (int i = 0; i < (int)str.length(); ++i)
	{
		if (str[i] == '(')
		{
			++leftBracketCount;
		}
		else if (str[i] == ')')
		{
			++rightBracketCount;
		}
	}
	if (leftBracketCount != rightBracketCount)
	{
		// 计算错误,左右括号数量不对应
		return 0.0f;
	}

	// 循环判断传入的字符串有没有括号
	while (true)
	{
		// 先判断有没有括号，如果有括号就先算括号里的,如果没有就退出while循环
		if (str.find_first_of("(") != std::string::npos || str.find_first_of(")") != std::string::npos)
		{
			int curpos = str.find_last_of("(");
			std::string strInBracket = str.substr(curpos + 1, str.length() - curpos - 1);
			strInBracket = strInBracket.substr(0, strInBracket.find_first_of(")"));
			float ret = calculateFloat(strInBracket);
			// 如果括号中的计算结果是负数,则标记为负数
			bool isMinus = false;
			if (ret < 0)
			{
				ret = -ret;
				isMinus = true;
			}
			// 将括号中的计算结果替换原来的表达式,包括括号也一起替换
			std::string floatStr = floatToString(ret, 4);
			str = strReplace(str, curpos, curpos + strInBracket.length() + 2, floatStr);
			if (isMinus)
			{
				// 如果括号中计算出来是负数,则将负号提取出来,将左边的第一个加减号改为相反的符号
				bool changeMark = false;
				for (int i = curpos - 1; i >= 0; --i)
				{
					// 找到第一个+号,则直接改为减号,然后退出遍历
					if (str[i] == '+')
					{
						str[i] = '-';
						changeMark = true;
						break;
					}
					// 找到第一个减号,如果减号的左边有数字,则直接改为+号
					// 如果减号的左边不是数字,则该减号是负号,将减号去掉,
					else if (str[i] == '-')
					{
						if (str[i - 1] >= '0' && str[i - 1] <= '9')
						{
							str[i] = '+';
						}
						else
						{
							str = strReplace(str, i, i + 1, EMPTY_STRING);
						}
						changeMark = true;
						break;
					}
				}
				// 如果遍历完了还没有找到可以替换的符号,则在表达式最前面加一个负号
				if (!changeMark)
				{
					str = "-" + str;
				}
			}
		}
		else
		{
			break;
		}
	}

	txVector<float> numbers;
	txVector<char> factors;
	// 表示上一个运算符的下标+1
	int beginpos = 0;
	for (int i = 0; i < (int)str.length(); ++i)
	{
		// 遍历到了最后一个字符,则直接把最后一个数字放入列表,然后退出循环
		if (i == (int)str.length() - 1)
		{
			std::string num = str.substr(beginpos, str.length() - beginpos);
			float fNum = stringToFloat(num);
			numbers.push_back(fNum);
			break;
		}
		// 找到第一个运算符
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.')
		{
			if (i != 0)
			{
				std::string num = str.substr(beginpos, i - beginpos);
				float fNum = stringToFloat(num);
				numbers.push_back(fNum);
			}
			// 如果在表达式的开始就发现了运算符,则表示第一个数是负数,那就处理为0减去这个数的绝对值
			else
			{
				numbers.push_back(0);
			}
			factors.push_back(str[i]);
			beginpos = i + 1;
		}
	}
	if (factors.size() + 1 != numbers.size())
	{
		// 计算错误,运算符与数字数量不符
		return 0;
	}
	// 现在开始计算表达式,按照运算优先级,先计算乘除和取余
	while (true)
	{
		// 表示是否还有乘除表达式
		bool hasMS = false;
		for (int i = 0; i < (int)factors.size(); ++i)
		{
			// 先遍历到哪个就先计算哪个
			if (factors[i] == '*' || factors[i] == '/')
			{
				// 第一个运算数的下标与运算符的下标是相同的
				float num1 = numbers[i];
				float num2 = numbers[i + 1];
				float num3 = 0.0f;
				if (factors[i] == '*')
				{
					num3 = num1 * num2;
				}
				else if (factors[i] == '/')
				{
					num3 = num1 / num2;
				}
				// 删除第i + 1个数,然后将第i个数替换为计算结果
				numbers.erase(numbers.begin() + i + 1);
				if (numbers.size() == 0)
				{
					// 计算错误
					return 0;
				}
				numbers[i] = num3;
				// 删除第i个运算符
				factors.erase(factors.begin() + i);
				hasMS = true;
				break;
			}
		}
		if (!hasMS)
		{
			break;
		}
	}
	// 再计算加减法
	while (true)
	{
		if (factors.size() == 0)
		{
			break;
		}
		if (factors[0] == '+' || factors[0] == '-')
		{
			// 第一个运算数的下标与运算符的下标是相同的
			float num1 = numbers[0];
			float num2 = numbers[1];
			float num3 = 0.0f;
			if (factors[0] == '+')
			{
				num3 = num1 + num2;
			}
			else if (factors[0] == '-')
			{
				num3 = num1 - num2;
			}
			// 删除第1个数,然后将第0个数替换为计算结果
			numbers.erase(numbers.begin() + 1);
			if (numbers.size() == 0)
			{
				// 计算错误
				return 0;
			}
			numbers[0] = num3;
			// 删除第0个运算符
			factors.erase(factors.begin());
		}
	}
	if (numbers.size() != 1)
	{
		// 计算错误
		return 0;
	}
	else
	{
		return numbers[0];
	}
}

int txUtility::calculateInt(std::string str)
{
	// 判断字符串是否含有非法字符,也就是除数字,小数点,运算符以外的字符
	for (int i = 0; i < (int)str.length();)
	{
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.' && str[i] != '+'
			&& str[i] != '-' && str[i] != '*' && str[i] != '/' && str[i] != '%'
			&& str[i] != '(' && str[i] != ')')
		{
			str.erase(i, 1);
		}
		else
		{
			++i;
		}
	}
	// 判断左右括号数量是否相等
	int leftBracketCount = 0;
	int rightBracketCount = 0;
	for (int i = 0; i < (int)str.length(); ++i)
	{
		if (str[i] == '(')
		{
			++leftBracketCount;
		}
		else if (str[i] == ')')
		{
			++rightBracketCount;
		}
	}
	if (leftBracketCount != rightBracketCount)
	{
		// 计算错误,左右括号数量不对应
		return 0;
	}

	// 循环判断传入的字符串有没有括号
	while (true)
	{
		// 先判断有没有括号，如果有括号就先算括号里的,如果没有就退出while循环
		if (str.find_first_of("(") != std::string::npos || str.find_first_of(")") != std::string::npos)
		{
			int curpos = str.find_last_of("(");
			std::string strInBracket = str.substr(curpos + 1, str.length() - curpos - 1);
			strInBracket = strInBracket.substr(0, strInBracket.find_first_of(")"));
			int ret = calculateInt(strInBracket);
			// 如果括号中的计算结果是负数,则标记为负数
			bool isMinus = false;
			if (ret < 0)
			{
				ret = -ret;
				isMinus = true;
			}
			// 将括号中的计算结果替换原来的表达式,包括括号也一起替换
			std::string intStr = intToString(ret, 4);
			str = strReplace(str, curpos, curpos + strInBracket.length() + 2, intStr);
			if (isMinus)
			{
				// 如果括号中计算出来是负数,则将负号提取出来,将左边的第一个加减号改为相反的符号
				bool changeMark = false;
				for (int i = curpos - 1; i >= 0; --i)
				{
					// 找到第一个+号,则直接改为减号,然后退出遍历
					if (str[i] == '+')
					{
						str[i] = '-';
						changeMark = true;
						break;
					}
					// 找到第一个减号,如果减号的左边有数字,则直接改为+号
					// 如果减号的左边不是数字,则该减号是负号,将减号去掉,
					else if (str[i] == '-')
					{
						if (str[i - 1] >= '0' && str[i - 1] <= '9')
						{
							str[i] = '+';
						}
						else
						{
							str = strReplace(str, i, i + 1, EMPTY_STRING);
						}
						changeMark = true;
						break;
					}
				}
				// 如果遍历完了还没有找到可以替换的符号,则在表达式最前面加一个负号
				if (!changeMark)
				{
					str = "-" + str;
				}
			}
		}
		else
		{
			break;
		}
	}

	txVector<int> numbers;
	txVector<char> factors;
	// 表示上一个运算符的下标+1
	int beginpos = 0;
	for (int i = 0; i < (int)str.length(); ++i)
	{
		// 遍历到了最后一个字符,则直接把最后一个数字放入列表,然后退出循环
		if (i == (int)str.length() - 1)
		{
			std::string num = str.substr(beginpos, str.length() - beginpos);
			int iNum = stringToInt(num);
			numbers.push_back(iNum);
			break;
		}
		// 找到第一个运算符
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.')
		{
			if (i != 0)
			{
				std::string num = str.substr(beginpos, i - beginpos);
				int iNum = stringToInt(num);
				numbers.push_back(iNum);
			}
			// 如果在表达式的开始就发现了运算符,则表示第一个数是负数,那就处理为0减去这个数的绝对值
			else
			{
				numbers.push_back(0);
			}
			factors.push_back(str[i]);
			beginpos = i + 1;
		}
	}
	if (factors.size() + 1 != numbers.size())
	{
		// 计算错误,运算符与数字数量不符
		return 0;
	}
	// 现在开始计算表达式,按照运算优先级,先计算乘除和取余
	while (true)
	{
		// 表示是否还有乘除表达式
		bool hasMS = false;
		for (int i = 0; i < (int)factors.size(); ++i)
		{
			// 先遍历到哪个就先计算哪个
			if (factors[i] == '*' || factors[i] == '/' || factors[i] == '%')
			{
				// 第一个运算数的下标与运算符的下标是相同的
				int num1 = numbers[i];
				int num2 = numbers[i + 1];
				int num3 = 0;
				if (factors[i] == '*')
				{
					num3 = num1 * num2;
				}
				else if (factors[i] == '/')
				{
					num3 = num1 / num2;
				}
				else if (factors[i] == '%')
				{
					num3 = num1 % num2;
				}
				// 删除第i + 1个数,然后将第i个数替换为计算结果
				numbers.erase(numbers.begin() + i + 1);
				if (numbers.size() == 0)
				{
					// 计算错误
					return 0;
				}
				numbers[i] = num3;
				// 删除第i个运算符
				factors.erase(factors.begin() + i);
				hasMS = true;
				break;
			}
		}
		if (!hasMS)
		{
			break;
		}
	}
	// 再计算加减法
	while (true)
	{
		if (factors.size() == 0)
		{
			break;
		}
		if (factors[0] == '+' || factors[0] == '-')
		{
			// 第一个运算数的下标与运算符的下标是相同的
			int num1 = numbers[0];
			int num2 = numbers[1];
			int num3 = 0;
			if (factors[0] == '+')
			{
				num3 = num1 + num2;
			}
			else if (factors[0] == '-')
			{
				num3 = num1 - num2;
			}
			// 删除第1个数,然后将第0个数替换为计算结果
			numbers.erase(numbers.begin() + 1);
			if (numbers.size() == 0)
			{
				// 计算错误
				return 0;
			}
			numbers[0] = num3;
			// 删除第0个运算符
			factors.erase(factors.begin());
		}
	}
	if (numbers.size() != 1)
	{
		// 计算错误
		return 0;
	}
	else
	{
		return numbers[0];
	}
}

std::string txUtility::intToString(const int& i, const int& limitLen)
{
	char str[256];
	SPRINTF(str, 256, "%d", i);
	std::string retString(str);
	int addLen = limitLen - strlen(str);
	if (addLen > 0)
	{
		char addStr[256];
		memset(addStr, 0, 256);
		for (int j = 0; j < addLen; ++j)
		{
			addStr[j] = '0';
		}
		retString = std::string(addStr) + retString;
	}

	return retString;
}

float txUtility::pow_f(const float& f, int p)
{
	float ret = 1.0f;
	while (p)
	{
		ret *= f;
		--p;
	}
	return ret;
}

std::string txUtility::floatToString(float f, const int& precision)
{
	f = f * pow_f(10.0f, precision) + 0.5f;
	int MAX_INT = 0x7FFFFFFF;
	if (f > (float)MAX_INT)
	{
		char str[256];
		SPRINTF(str, 256, "%f", f);
		return std::string(str);
	}

	int intValue = (int)f;

	char str[256];
	SPRINTF(str, 256, "%d", intValue);
	if (precision == 0)
	{
		return std::string(str);
	}
	else
	{
		int length = strlen(str);
		if (length <= precision)
		{
			char newStr[256];
			memset(newStr, 0, 256 * sizeof(char));
			for (int i = 0; i < precision - length + 1; ++i)
			{
				newStr[i] = '0';
			}
			memcpy(newStr + precision - length + 1, str, length);

			char intStr[256];
			memset(intStr, 0, 256 * sizeof(char));
			char decimalStr[256];
			memset(decimalStr, 0, 256 * sizeof(char));
			memcpy(intStr, newStr, strlen(newStr) - precision);
			memcpy(decimalStr, newStr + strlen(newStr) - precision, precision);
			return std::string(intStr) + "." + std::string(decimalStr);
		}
		else
		{
			char intStr[256];
			memset(intStr, 0, 256 * sizeof(char));
			char decimalStr[256];
			memset(decimalStr, 0, 256 * sizeof(char));
			memcpy(intStr, str, strlen(str) - precision);
			memcpy(decimalStr, str + strlen(str) - precision, precision);
			return std::string(intStr) + "." + std::string(decimalStr);
		}
	}
}

#if RUN_PLATFORM == PLATFORM_WINDOWS
std::wstring txUtility::ANSIToUnicode(const std::string& str)
{
	 int len = str.length();
	 int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	 wchar_t* pUnicode = TRACE_NEW_ARRAY(wchar_t, unicodeLen + 1, pUnicode);
	 ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);
	 std::wstring rt = (wchar_t*)pUnicode;
	 TRACE_DELETE_ARRAY(pUnicode);
	 return rt;
}

std::string txUtility::UnicodeToANSI(const std::wstring& str)
{
	int iTextLen = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	char* pElementText = TRACE_NEW_ARRAY(char, iTextLen + 1, pElementText);
	::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, pElementText, iTextLen, NULL, NULL);
	std::string strText = pElementText;
	TRACE_DELETE_ARRAY(pElementText);
	return strText;
}
std::string txUtility::UnicodeToUTF8(const std::wstring& str)
 {
	// wide char to multi char
	int iTextLen = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	char* pElementText = TRACE_NEW_ARRAY(char, iTextLen + 1, pElementText);
	::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, pElementText, iTextLen, NULL, NULL);
	std::string strText = pElementText;
	TRACE_DELETE_ARRAY(pElementText);
	return strText;
 }

std::wstring txUtility::UTF8ToUnicode(const std::string& str)
 {
	 int len = str.length();
	 int unicodeLen = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	 wchar_t* pUnicode = TRACE_NEW_ARRAY(wchar_t, unicodeLen + 1, pUnicode);
	 ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);
	 std::wstring rt = (wchar_t*)pUnicode;
	 TRACE_DELETE_ARRAY(pUnicode);
	 return rt;
}

std::string txUtility::ANSIToUTF8(const std::string& str)
{
	std::wstring unicodeStr = ANSIToUnicode(str);
	std::string utf8Str = UnicodeToUTF8(unicodeStr);
	return utf8Str;
}

std::string txUtility::UTF8ToANSI(const std::string& str)
{
	std::wstring unicodeStr = UTF8ToUnicode(str);
	std::string ansiStr = UnicodeToANSI(unicodeStr);
	return ansiStr;
}
#endif


#if RUN_PLATFORM == PLATFORM_WINDOWS
bool txUtility::copyFile(const std::string& sourceFile, const std::string& destFile, bool overWrite)
{
	// 如果目标文件所在的目录不存在,则先创建目录
	std::string parentDir = getParentDir(destFile);
	createFolder(parentDir);
	return CopyFileA(sourceFile.c_str(), destFile.c_str(), !overWrite) == TRUE;
}
#elif RUN_PLATFORM == PLATFORM_LINUX
bool txUtility::copyFile(const std::string& sourceFile, const std::string& destFile, bool overWrite)
{
	return false;
}
#endif

bool txUtility::createFolder(const std::string& path)
{
	// 如果有上一级目录,并且上一级目录不存在,则先创建上一级目录
	std::string parentDir = getParentDir(path);
	if (parentDir != path)
	{
		createFolder(parentDir);
	}
	// 如果文件不存在则创建文件
	if (!isFileExist(path))
	{
#if RUN_PLATFORM == PLATFORM_WINDOWS
		if (0 != _mkdir(path.c_str()))
#elif RUN_PLATFORM == PLATFORM_LINUX
		if (0 != mkdir(path.c_str(), S_IRUSR | S_IWUSR | S_IXUSR))
#endif
		{
			return false;
		}
	}
	return true;
}

bool txUtility::writeFile(std::string filePath, const int& length, const char* buffer)
{
	if (length <= 0 || NULL == buffer)
	{
		GAME_ERROR("error : file length is 0 or buffer is NULL! can not write file! file path : %s", filePath.c_str());
		return false;
	}

	txUtility::rightToLeft(filePath);

	int pos = filePath.find_last_of('/');
	if (-1 != pos)
	{
		std::string dirPath = filePath.substr(0, pos);
		if (!createFolder(dirPath))
		{
			GAME_ERROR("error : can not create folder, name : %s", dirPath.c_str());
			return false;
		}
	}
	else
	{
		if (!createFolder(filePath))
		{
			GAME_ERROR("error : can not create folder, name : %s", filePath.c_str());
			return false;
		}
	}

#if RUN_PLATFORM == PLATFORM_WINDOWS
	FILE* pFile = NULL;
	fopen_s(&pFile, filePath.c_str(), "wb");
#elif RUN_PLATFORM == PLATFORM_LINUX
	FILE* pFile = fopen(filePath.c_str(), "wb");
#endif

	if (pFile == NULL)
	{
		GAME_ERROR("error : can not write file, name : %s", filePath.c_str());
		return false;
	}

	fwrite(buffer, sizeof(char), length, pFile);
	fclose(pFile);

	return true;
}
int txUtility::strFind(const std::string& res, const std::string& sub, const int& startPose, const bool& fromStart)
{
	int subLen = (int)sub.length();
	int searchLength = res.length() - subLen;
	if (fromStart)
	{
		int j = 0;
		for (int i = startPose; i < searchLength; ++i)
		{
			for (j = 0; j != subLen; ++j)
			{
				if (res[i + j] != sub[j])
				{
					break;
				}
			}

			if (j == subLen)
			{
				return i;
			}
		}
	}
	else
	{
		int j = 0;
		for (int i = searchLength; i != startPose; --i)
		{
			for (j = 0; j != subLen; ++j)
			{
				if (res[i + j] != sub[j])
				{
					break;
				}
			}

			if (j == subLen)
			{
				return i;
			}
		}
	}
	return -1;
}

bool txUtility::findSubstr(std::string res, std::string dst, const bool& sensitive, int* pos, const int& startPose, const bool& firstOrLast)
{
	// 如果不区分大小写
	if (!sensitive)
	{
		// 全转换为小写
		strToLower(res);
		strToLower(dst);
	}
	int posFind = strFind(res, dst, startPose, firstOrLast);
	if (NULL != pos)
	{
		*pos = posFind;
	}
	if (-1 != posFind)
	{
		return true;
	}
	return false;
}

void txUtility::strToLower(std::string& str)
{
	std::string::size_type size = str.length();
	for (std::string::size_type i = 0; i != size; ++i)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
		{
			str[i] += 'a' - 'A';
		}
	}
}

void txUtility::strToUpper(std::string& str)
{
	std::string::size_type size = str.length();
	for (std::string::size_type i = 0; i != size; ++i)
	{
		if (str[i] >= 'a' && str[i] <= 'z')
		{
			str[i] -= 'a' - 'A';
		}
	}
}
