#ifndef _TX_FILE_UTILITY_H_
#define _TX_FILE_UTILITY_H_

#include "ServerDefine.h"

class txFileUtility
{
public:
	static std::string validPath(const std::string& path);
	static void findFiles(const std::string& path, txVector<std::string>& files, const std::string& patterns, bool recursive = true);
#if RUN_PLATFORM == PLATFORM_ANDROID
	static bool isDirectory(const std::string& path);
#endif
	static void findFiles(const std::string& path, txVector<std::string>& files, const txVector<std::string>& patterns = txVector<std::string>(), bool recursive = true);
	static void findFolders(const std::string& path, txVector<std::string>& folders, bool recursive = false);
	static void deleteFolder(const std::string& path);
	static bool isFileExist(const std::string& fullPath);
	// 将sourceFile拷贝到destFile,sourceFile和destFile都是带可直接访问的路径的文件名,overWrite指定当目标文件已经存在时是否要覆盖文件
	static bool copyFile(const std::string& sourceFile, const std::string& destFile, bool overWrite = true);
	// 创建一个文件夹,path是一个不以/结尾的可直接访问的相对或者绝对的文件夹名
	static bool createFolder(const std::string& path);
	static bool writeFile(std::string filePath, int length, const char* buffer);
	static char* openFile(const std::string& filePath, int* bufferSize, bool addZero);
	static std::string openTxtFile(const std::string& filePath);
	static char* openBinaryFile(const std::string& filePath, int* bufferSize);
};

#endif