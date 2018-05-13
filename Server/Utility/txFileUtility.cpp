#include "txFileUtility.h"
#include "txStringUtility.h"
#include "GameLog.h"
#include "txUtility.h"

std::string txFileUtility::validPath(const std::string& path)
{
	std::string temp = path;
	if (temp.length() > 0)
	{
		// ����/��β,�����/
		if (temp[temp.length() - 1] != '/')
		{
			temp += "/";
		}
	}
	return temp;
}

void txFileUtility::findFiles(const std::string& pathName, txVector<std::string>& files, const std::string& patterns, const bool& recursive)
{
	txVector<std::string> patternList;
	patternList.push_back(patterns);
	findFiles(pathName, files, patternList, recursive);
}
#if RUN_PLATFORM == PLATFORM_ANDROID
// �ж��Ƿ�ΪĿ¼
bool txFileUtility::isDirectory(const std::string& pszName)
{
	struct stat S_stat;
	// ȡ���ļ�״̬
	if (lstat(pszName.c_str(), &S_stat) < 0)
	{
		return false;
	}
	// �ж��ļ��Ƿ�Ϊ�ļ���
	return S_ISDIR(S_stat.st_mode);
}

void txFileUtility::findFiles(const std::string& path, txVector<std::string>& files, const txVector<std::string>& patterns, const bool& recursive)
{
#ifdef LOAD_FROM_ASSETMANAGER
	txVector<std::string> fileName = ASS_getFileList((char*)path.c_str());
	int fileCount = fileName.size();
	FOR_STL(fileName, int i = 0; i < fileCount; ++i)
	{
		const std::string& fullName = fileName[i];
		// �ж��Ƿ���Ϻ�׺����
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
		//�����.����..����
		if (std::string(pDirent->d_name) == "." || std::string(pDirent->d_name) == "..")
		{
			continue;
		}
		//�ж��Ƿ�Ϊ�ļ���
		memset(szTmpPath, 0, 1024);
		SPRINTF(szTmpPath, 1024, "%s/%s", path.c_str(), pDirent->d_name);
		if (isDirectory(szTmpPath))
		{
			if (recursive)
			{
				//������ļ�������еݹ�
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

void txFileUtility::findFolders(const std::string& path, txVector<std::string>& folders, const bool& recursive)
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
		//�����.����..����
		if (std::string(pDirent->d_name) == "." || std::string(pDirent->d_name) == "..")
		{
			continue;
		}
		//�ж��Ƿ�Ϊ�ļ���
		memset(szTmpPath, 0, 1024);
		SPRINTF(szTmpPath, 1024, "%s/%s", path.c_str(), pDirent->d_name);
		// ������ļ���,���Ƚ��ļ��з����б�,Ȼ���ж��Ƿ�ݹ����
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

#elif RUN_PLATFORM == PLATFORM_WINDOWS
void txFileUtility::findFiles(const std::string& path, txVector<std::string>& files, const txVector<std::string>& patterns, const bool& recursive)
{
	std::string tempPath = validPath(path);
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = FindFirstFileA((tempPath + "*").c_str(), &FindFileData);
	// ����Ҳ����ļ��о�ֱ�ӷ���
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}
	do
	{
		// ����.��..
		if (strcmp(FindFileData.cFileName, ".") == 0
			|| strcmp(FindFileData.cFileName, "..") == 0)
		{
			continue;
		}

		// ��������·��
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
					if (txStringUtility::endWith(fullname, patterns[i], false))
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

void txFileUtility::findFolders(const std::string& path, txVector<std::string>& folders, const bool& recursive)
{
	std::string tempPath = validPath(path);
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = FindFirstFileA((tempPath + "*").c_str(), &FindFileData);
	// ����Ҳ����ļ��о�ֱ�ӷ���
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}
	do
	{
		// ����.��..
		if (strcmp(FindFileData.cFileName, ".") == 0
			|| strcmp(FindFileData.cFileName, "..") == 0)
		{
			continue;
		}

		// ��������·��
		std::string fullname = tempPath + std::string(FindFileData.cFileName);
		// ���ļ������ȷ����б�,Ȼ���ж��Ƿ�ݹ����
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

void txFileUtility::deleteFolder(const std::string& path)
{
	WIN32_FIND_DATAA FindData;
	// ����·��
	std::string pathName = path + "/*.*";
	HANDLE hFind = FindFirstFileA(pathName.c_str(), &FindData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}
	while (::FindNextFileA(hFind, &FindData))
	{
		// ����.��..
		if (strcmp(FindData.cFileName, ".") == 0
			|| strcmp(FindData.cFileName, "..") == 0)
		{
			continue;
		}

		// ��������·��
		std::string fullname = path + "/" + std::string(FindData.cFileName);
		// ������ļ���,��ݹ�ɾ���ļ���
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			deleteFolder(fullname);
		}
		// ������ļ�,��ֱ��ɾ���ļ�
		else
		{
			DeleteFileA(fullname.c_str());
		}
	}
	::FindClose(hFind);
	// ɾ���ļ�������
	RemoveDirectoryA(path.c_str());
}
#endif

bool txFileUtility::isFileExist(const std::string& fullPath)
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

bool txFileUtility::copyFile(const std::string& sourceFile, const std::string& destFile, bool overWrite)
{
	// ���Ŀ���ļ����ڵ�Ŀ¼������,���ȴ���Ŀ¼
	std::string parentDir = txStringUtility::getFilePath(destFile);
	createFolder(parentDir);
#if RUN_PLATFORM == PLATFORM_WINDOWS
	return CopyFileA(sourceFile.c_str(), destFile.c_str(), !overWrite) == TRUE;
#elif RUN_PLATFORM == PLATFORM_ANDROID
	return false;
#endif
}

bool txFileUtility::createFolder(const std::string& path)
{
	// �������һ��Ŀ¼,������һ��Ŀ¼������,���ȴ�����һ��Ŀ¼
	std::string parentDir = txStringUtility::getFilePath(path);
	if (parentDir != path)
	{
		createFolder(parentDir);
	}
	// ����ļ��������򴴽��ļ�
	if (!isFileExist(path))
	{
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

bool txFileUtility::writeFile(std::string filePath, const int& length, const char* buffer)
{
#ifdef LOAD_FROM_ASSETMANAGER
	return false;
#else

	if (length <= 0 || NULL == buffer)
	{
		LOG_ERROR("error : file length is 0 or buffer is NULL! can not write file! file path : %s", filePath.c_str());
		return false;
	}

	txStringUtility::rightToLeft(filePath);
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
	else
	{
		if (!createFolder(filePath))
		{
			LOG_ERROR("error : can not create folder, name : %s", filePath.c_str());
			return false;
		}
	}

#ifdef _USE_SAFE_API
	FILE* pFile = NULL;
	fopen_s(&pFile, filePath.c_str(), "wb");
#else
	FILE* pFile = fopen(filePath.c_str(), "wb");
#endif
	if (pFile == NULL)
	{
		LOG_ERROR("error : can not write file, name : %s", filePath.c_str());
		return false;
	}
	fwrite(buffer, sizeof(char), length, pFile);
	fclose(pFile);
#endif
	return true;
}

char* txFileUtility::openFile(const std::string& filePath, int* bufferSize, const bool& addZero)
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

std::string txFileUtility::openTxtFile(const std::string& filePath)
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

char* txFileUtility::openBinaryFile(const std::string& filePath, int* bufferSize)
{
	return openFile(filePath, bufferSize, false);
}