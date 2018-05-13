#ifndef _TX_FILE_UTILITY_H_
#define _TX_FILE_UTILITY_H_

#include "ServerDefine.h"

class txFileUtility
{
public:
	static std::string validPath(const std::string& path);
	static void findFiles(const std::string& path, txVector<std::string>& files, const std::string& patterns, const bool& recursive = true);
#if RUN_PLATFORM == PLATFORM_ANDROID
	static bool isDirectory(const std::string& path);
#endif
	static void findFiles(const std::string& path, txVector<std::string>& files, const txVector<std::string>& patterns = txVector<std::string>(), const bool& recursive = true);
	static void findFolders(const std::string& path, txVector<std::string>& folders, const bool& recursive = false);
	static void deleteFolder(const std::string& path);
	static bool isFileExist(const std::string& fullPath);
	// ��sourceFile������destFile,sourceFile��destFile���Ǵ���ֱ�ӷ��ʵ�·�����ļ���,overWriteָ����Ŀ���ļ��Ѿ�����ʱ�Ƿ�Ҫ�����ļ�
	static bool copyFile(const std::string& sourceFile, const std::string& destFile, bool overWrite = true);
	// ����һ���ļ���,path��һ������/��β�Ŀ�ֱ�ӷ��ʵ���Ի��߾��Ե��ļ�����
	static bool createFolder(const std::string& path);
	static bool writeFile(std::string filePath, const int& length, const char* buffer);
	static char* openFile(const std::string& filePath, int* bufferSize, const bool& addZero);
	static std::string openTxtFile(const std::string& filePath);
	static char* openBinaryFile(const std::string& filePath, int* bufferSize);
};

#endif