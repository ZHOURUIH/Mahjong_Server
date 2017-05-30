#include "txDataManager.h"
#include "txUtility.h"
#include "ServerConfig.h"
#include "GameLog.h"

std::map<std::string, SERVER_DEFINE> ServerConfig::mFloatParamDefineList;
std::map<SERVER_DEFINE, float> ServerConfig::mFloatParamList;

#define ADD_FLOAT_PARAM(t) mFloatParamDefineList.insert(std::pair<std::string, SERVER_DEFINE>(TOSTRING(t), t));

ServerConfig::ServerConfig()
{
	ADD_FLOAT_PARAM(SD_SOCKET_PORT);
	ADD_FLOAT_PARAM(SD_HEART_BEAT_TIME_OUT);
	ADD_FLOAT_PARAM(SD_BACK_LOG);
	ADD_FLOAT_PARAM(SD_SHOW_COMMAND_DEBUG_INFO);
	ADD_FLOAT_PARAM(SD_OUTPUT_NET_LOG);
	if (mFloatParamDefineList.size() != SD_MAX)
	{
		GAME_ERROR("error : not all float parameters init! init count : %d, , max count : %d", (int)mFloatParamDefineList.size(), SD_MAX);
	}
}

void ServerConfig::readConfig(const std::string& fileName)
{
	int fileIndex = mDataManager->LoadData(fileName.c_str(), true);
	if (fileIndex == -1)
	{
		return;
	}
	txDataElem* dataElem = mDataManager->GetData(fileIndex);
	char* data = dataElem->getValuePtr();
#if RUN_PLATFORM == PLATFORM_WINDOWS
	char returnChar = '\r';
	const char* returnNextLine = "\r\n";
#elif RUN_PLATFORM == PLATFORM_LINUX
	char returnChar = '\n';
	const char* returnNextLine = "\n";
#endif
	// 将文件的每一行处理后放入lineList中
	std::vector<std::string> lineList;
	std::string dataString(data);
	while(true)
	{
		int returnPos = dataString.find_first_of(returnChar);
		if(returnPos == -1)
		{
			std::string lineString = dataString;
			if (lineString.length() != 0)
			{
				lineList.push_back(lineString);
			}
			break;
		}
		std::string lineString = dataString.substr(0, returnPos);
		if(lineString.length() != 0)
		{
			lineList.push_back(lineString);
		}
		dataString = dataString.substr(returnPos + strlen(returnNextLine), dataString.length() - returnPos - strlen(returnNextLine));
	}

	mDataManager->DestroyData(fileIndex);

	// 将数据字符串拆分出来,放入valueList中
	std::map<std::string, std::string> valueList;
	int lineCount = lineList.size();
	for (int i = 0; i < lineCount; ++i)
	{
		std::string lineString = lineList[i];
		// 首先去掉所有的空格和制表符
		char* newStringBuffer = TRACE_NEW_ARRAY(char, lineString.length() + 1, newStringBuffer);
		int curLen = 0;
		int strLength = lineString.length();
		for (int j = 0; j < strLength; ++j)
		{
			if (lineString[j] != ' ' && lineString[j] != '\t')
			{
				newStringBuffer[curLen] = lineString[j];
				++curLen;
			}
		}
		std::string newString(newStringBuffer);
		TRACE_DELETE_ARRAY(newStringBuffer);
		// 如果该行是空的,或者是注释,则不进行处理
		if (newString.length() > 0 && newString.substr(0, 2) != std::string("//"))
		{
			std::vector<std::string> valueVector;
			txUtility::split(newString, "=", &valueVector);
			if(valueVector.size() == 2)
			{
				valueList.insert(std::pair<std::string, std::string>(valueVector[0], valueVector[1]));
			}
		}
	}

	// 解析valueList中的数据字符串
	std::map<std::string, std::string>::iterator iterValue = valueList.begin();
	std::map<std::string, std::string>::iterator iterValueEnd = valueList.end();
	for (; iterValue != iterValueEnd; ++iterValue)
	{
		std::map<std::string, SERVER_DEFINE>::iterator iterDefine = mFloatParamDefineList.find(iterValue->first);
		if (iterDefine != mFloatParamDefineList.end())
		{
			setFloatParam(iterDefine->second, (float)atof(iterValue->second.c_str()));
		}
	}
}
