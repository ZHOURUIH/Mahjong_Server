﻿#include "Utility.h"

std::string FileUtility::validPath(const std::string& path)
{
	std::string temp = path;
	if (temp.length() > 0)
	{
		// 不以/结尾,则加上/
		if (temp[temp.length() - 1] != '/')
		{
			temp += "/";
		}
	}
	return temp;
}

void FileUtility::findFiles(const std::string& pathName, txVector<std::string>& files, const std::string& patterns, bool recursive)
{
	txVector<std::string> patternList;
	patternList.push_back(patterns);
	findFiles(pathName, files, patternList, recursive);
}
#if RUN_PLATFORM == PLATFORM_ANDROID
// 判断是否为目录
bool FileUtility::isDirectory(const std::string& pszName)
{
	struct stat S_stat;
	// 取得文件状态
	if (lstat(pszName.c_str(), &S_stat) < 0)
	{
		return false;
	}
	// 判断文件是否为文件夹
	return S_ISDIR(S_stat.st_mode);
}

void FileUtility::findFiles(const std::string& path, txVector<std::string>& files, const txVector<std::string>& patterns, bool recursive)
{
#ifdef LOAD_FROM_ASSETMANAGER
	txVector<std::string> fileName = ASS_getFileList((char*)path.c_str());
	int fileCount = fileName.size();
	FOR_STL(fileName, int i = 0; i < fileCount; ++i)
	{
		const std::string& fullName = fileName[i];
		// 判断是否符合后缀过滤
		int patternCount = patterns.size();
		if(patternCount > 0)
		{
			for(int i = 0; i < patternCount; ++i)
			{
				if (txStringUtility::endWith(fullName, patterns[j], false))
				{
					files.push_back(fullName);
					break;
				}
			}
		}
		else
		{
			files.push_back(fullName);
		}
	}
	END_FOR_STL(fileName);

#else
	DIR* pDir = opendir(path.c_str());
	if (pDir == NULL)
	{
		return;
	}
	char szTmpPath[1024];
	dirent* pDirent = NULL;
	while ((pDirent = readdir(pDir)) != NULL)
	{
		//如果是.或者..跳过
		if (std::string(pDirent->d_name) == "." || std::string(pDirent->d_name) == "..")
		{
			continue;
		}
		//判断是否为文件夹
		memset(szTmpPath, 0, 1024);
		SPRINTF(szTmpPath, 1024, "%s/%s", path.c_str(), pDirent->d_name);
		if (isDirectory(szTmpPath))
		{
			if (recursive)
			{
				//如果是文件夹则进行递归
				findFiles(szTmpPath, files, patterns, recursive);
			}
		}
		else
		{
			int patternCount = patterns.size();
			if(patternCount > 0)
			{
				for(int i = 0; i < patternCount; ++i)
				{
					if (txStringUtility::endWith(szTmpPath, patterns[i], false))
					{
						files.push_back(szTmpPath);
						break;
					}
				}
			}
			else
			{
				files.push_back(szTmpPath);
			}
		}
	}
	closedir(pDir);
#endif
}

void FileUtility::findFolders(const std::string& path, txVector<std::string>& folders, bool recursive)
{
	struct dirent* pDirent;
	DIR* pDir = opendir(path.c_str());
	if (pDir == NULL)
	{
		return;
	}
	char szTmpPath[1024];
	while ((pDirent = readdir(pDir)) != NULL)
	{
		//如果是.或者..跳过
		if (std::string(pDirent->d_name) == "." || std::string(pDirent->d_name) == "..")
		{
			continue;
		}
		//判断是否为文件夹
		memset(szTmpPath, 0, 1024);
		SPRINTF(szTmpPath, 1024, "%s/%s", path.c_str(), pDirent->d_name);
		// 如果是文件夹,则先将文件夹放入列表,然后判断是否递归查找
		if (isDirectory(szTmpPath))
		{
			folders.push_back(szTmpPath);
			if (recursive)
			{
				findFolders(szTmpPath, folders, recursive);
			}
		}
	}
	closedir(pDir);
}
void FileUtility::deleteFile(const std::string& path)
{
	remove(path.c_str());
}

#elif RUN_PLATFORM == PLATFORM_WINDOWS
void FileUtility::findFiles(const std::string& path, txVector<std::string>& files, const txVector<std::string>& patterns, bool recursive)
{
	std::string tempPath = validPath(path);
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = FindFirstFileA((tempPath + "*").c_str(), &FindFileData);
	// 如果找不到文件夹就直接返回
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}
	do
	{
		// 过滤.和..
		if (strcmp(FindFileData.cFileName, ".") == 0
			|| strcmp(FindFileData.cFileName, "..") == 0)
		{
			continue;
		}

		// 构造完整路径
		std::string fullname = tempPath + std::string(FindFileData.cFileName);
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
				for(int i = 0; i < patternCount; ++i)
				{
					if (StringUtility::endWith(fullname, patterns[i], false))
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
	}while (::FindNextFileA(hFind, &FindFileData));
	::FindClose(hFind);
}

void FileUtility::findFolders(const std::string& path, txVector<std::string>& folders, bool recursive)
{
	std::string tempPath = validPath(path);
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = FindFirstFileA((tempPath + "*").c_str(), &FindFileData);
	// 如果找不到文件夹就直接返回
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}
	do
	{
		// 过滤.和..
		if (strcmp(FindFileData.cFileName, ".") == 0
			|| strcmp(FindFileData.cFileName, "..") == 0)
		{
			continue;
		}

		// 构造完整路径
		std::string fullname = tempPath + std::string(FindFileData.cFileName);
		// 是文件夹则先放入列表,然后判断是否递归查找
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			folders.push_back(fullname);
			if (recursive)
			{
				findFolders(fullname.c_str(), folders, recursive);
			}
		}
	} while (::FindNextFileA(hFind, &FindFileData));
	::FindClose(hFind);
}

void FileUtility::deleteFolder(const std::string& path)
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

void FileUtility::deleteFile(const std::string& path)
{
	DeleteFileA(path.c_str());
}
#endif

bool FileUtility::isFileExist(const std::string& fullPath)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	int ret = _access(fullPath.c_str(), 0);
#elif RUN_PLATFORM == PLATFORM_ANDROID
#ifdef LOAD_FROM_ASSETMANAGER
	int ret = ASS_isFileExist((char*)fullPath.c_str()) ? 0 : -1;
#else
	int ret = access(fullPath.c_str(), 0);
#endif
#endif
	return ret == 0;
}

bool FileUtility::copyFile(const std::string& sourceFile, const std::string& destFile, bool overWrite)
{
	// 如果目标文件所在的目录不存在,则先创建目录
	std::string parentDir = StringUtility::getFilePath(destFile);
	createFolder(parentDir);
#if RUN_PLATFORM == PLATFORM_WINDOWS
	return CopyFileA(sourceFile.c_str(), destFile.c_str(), !overWrite) == TRUE;
#elif RUN_PLATFORM == PLATFORM_ANDROID
	return false;
#endif
}

bool FileUtility::createFolder(const std::string& path)
{
	// 如果目录已经存在,则返回true
	if (isFileExist(path))
	{
		return true;
	}
	// 如果文件不存在则检查父目录是否存在
	if (!isFileExist(path))
	{
		// 如果有上一级目录,并且上一级目录不存在,则先创建上一级目录
		std::string parentDir = StringUtility::getFilePath(path);
		if (parentDir != path)
		{
			createFolder(parentDir);
		}
#ifdef _USE_SAFE_API
		if (0 != _mkdir(path.c_str()))
#else
		if (0 != mkdir(path.c_str(), S_IRUSR | S_IWUSR | S_IXUSR))
#endif
		{
			return false;
		}
	}
	return true;
}

bool FileUtility::writeFile(std::string filePath, const std::string& text, bool append)
{
	return writeFile(filePath, text.c_str(), text.length(), append);
}

bool FileUtility::writeFile(std::string filePath, const char* buffer, int length, bool append)
{
#ifdef LOAD_FROM_ASSETMANAGER
	return false;
#endif
	// 检查参数
	if (length < 0 || buffer == NULL)
	{
		LOG_ERROR("error : file length error! can not write file! file path : %s", filePath.c_str());
		return false;
	}
	if (length > 0 && buffer == NULL)
	{
		LOG_ERROR("error : buffer is NULL! can not write file! file path : %s", filePath.c_str());
		return false;
	}
	// 检查路径
	StringUtility::rightToLeft(filePath);
	int pos = filePath.find_last_of('/');
	if (pos != -1)
	{
		std::string dirPath = filePath.substr(0, pos);
		if (!createFolder(dirPath))
		{
			LOG_ERROR("error : can not create folder, name : %s", dirPath.c_str());
			return false;
		}
	}
	writeFileSimple(filePath, buffer, length, append);
	return true;
}

bool FileUtility::writeFileSimple(const std::string& fileName, const char* buffer, int writeCount, bool append)
{
#ifdef _USE_SAFE_API
	FILE* pFile = NULL;
	fopen_s(&pFile, fileName.c_str(), "a+");
#else
	FILE* pFile = fopen(filePath.c_str(), "a+");
#endif
	if (pFile == NULL)
	{
		LOG_ERROR("error : can not write file, name : %s", fileName.c_str());
		return false;
	}
	if (append)
	{
		fseek(pFile, 0, SEEK_END);
	}
	fwrite(buffer, sizeof(char), writeCount, pFile);
	fclose(pFile);
	return true;
}

bool FileUtility::writeFileSimple(const std::string& fileName, const std::string& text, bool append)
{
	return writeFileSimple(fileName, text.c_str(), text.length(), append);
}

char* FileUtility::openFile(const std::string& filePath, int* bufferSize, bool addZero)
{
	FILE* pFile = NULL;
#if RUN_PLATFORM == PLATFORM_WINDOWS
	fopen_s(&pFile, filePath.c_str(), "rb");
#elif RUN_PLATFORM == PLATFORM_LINUX
	pFile = fopen(filePath.c_str(), "rb");
#endif
	if (pFile == NULL)
	{
		return NULL;
	}
	fseek(pFile, 0, SEEK_END);
	int fileSize = ftell(pFile);
	rewind(pFile);
	int bufferLen = addZero ? fileSize + 1 : fileSize;
	if (bufferSize != NULL)
	{
		*bufferSize = bufferLen;
	}
	char* buffer = TRACE_NEW_ARRAY(char, bufferLen, buffer);
	if (buffer != NULL)
	{
		fread(buffer, sizeof(char), fileSize, pFile);
	}
	fclose(pFile);
	return buffer;
}

std::string FileUtility::openTxtFile(const std::string& filePath)
{
	char* buffer = openFile(filePath, NULL, true);
	if (buffer == NULL)
	{
		return EMPTY_STRING;
	}
	std::string str(buffer);
	TRACE_DELETE_ARRAY(buffer);
	return str;
}

char* FileUtility::openBinaryFile(const std::string& filePath, int* bufferSize)
{
	return openFile(filePath, bufferSize, false);
}