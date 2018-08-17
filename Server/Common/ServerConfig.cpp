#include "Utility.h"
#include "ServerConfig.h"
#include "GameLog.h"

txMap<std::string, SERVER_DEFINE_FLOAT> ServerConfig::mFloatParamDefineList;
txMap<std::string, SERVER_DEFINE_STRING> ServerConfig::mStringParamDefineList;
txMap<SERVER_DEFINE_FLOAT, float> ServerConfig::mFloatParamList;
txMap<SERVER_DEFINE_STRING, std::string> ServerConfig::mStringParamList;

#define ADD_FLOAT_PARAM(t) mFloatParamDefineList.insert(TOSTRING(t), t);
#define ADD_STRING_PARAM(t) mStringParamDefineList.insert(TOSTRING(t), t);

ServerConfig::ServerConfig()
{
	ADD_FLOAT_PARAM(SDF_SOCKET_PORT);
	ADD_FLOAT_PARAM(SDF_HEART_BEAT_TIME_OUT);
	ADD_FLOAT_PARAM(SDF_BACK_LOG);
	ADD_FLOAT_PARAM(SDF_SHOW_COMMAND_DEBUG_INFO);
	ADD_FLOAT_PARAM(SDF_OUTPUT_NET_LOG);
	if (mFloatParamDefineList.size() != SDF_MAX)
	{
		LOG_ERROR("error : not all float parameters init! init count : %d, , max count : %d", (int)mFloatParamDefineList.size(), SDF_MAX);
	}

	ADD_STRING_PARAM(SDS_DOMAIN_NAME);
	if (mStringParamDefineList.size() != SDS_MAX)
	{
		LOG_ERROR("error : not all string parameters init! init count : %d, , max count : %d", (int)mStringParamDefineList.size(), SDS_MAX);
	}
}

void ServerConfig::init()
{
	readConfig(CONFIG_PATH + "GameFloatConfig.txt", true); 
	readConfig(CONFIG_PATH + "GameStringConfig.txt", false);
}

void ServerConfig::readConfig(const std::string& fileName, bool floatParam)
{
	std::string dataString = txFileUtility::openTxtFile(txUtility::getAvailableResourcePath(fileName));
#if RUN_PLATFORM == PLATFORM_WINDOWS
	char returnChar = '\r';
	const char* returnNextLine = "\r\n";
#elif RUN_PLATFORM == PLATFORM_LINUX
	char returnChar = '\n';
	const char* returnNextLine = "\n";
#endif
	// 将文件的每一行处理后放入lineList中
	txVector<std::string> lineList;
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

	// 每行最多只允许2048个字节
	const int LINE_MAX_LENGTH = 2048;
	char* tempBuffer = TRACE_NEW_ARRAY(char, LINE_MAX_LENGTH, tempBuffer);
	// 将数据字符串拆分出来,放入valueList中
	txMap<std::string, std::string> valueList;
	int lineCount = lineList.size();
	FOR_STL(lineList, int i = 0; i < lineCount; ++i)
	{
		const std::string& lineString = lineList[i];
		// 首先去掉所有的空格和制表符
		int curLen = 0;
		int strLength = lineString.length();
		for (int j = 0; j < strLength; ++j)
		{
			if (lineString[j] != ' ' && lineString[j] != '\t')
			{
				if (curLen >= LINE_MAX_LENGTH)
				{
					break;
				}
				tempBuffer[curLen] = lineString[j];
				++curLen;
			}
		}
		tempBuffer[curLen] = 0;
		std::string newString(tempBuffer);
		// 如果该行是空的,或者是注释,则不进行处理
		if (newString.length() > 0 && !txStringUtility::startWith(newString, "//"))
		{
			txVector<std::string> valueVector;
			txStringUtility::split(newString, "=", valueVector);
			if (valueVector.size() == 2)
			{
				valueList.insert(valueVector[0], valueVector[1]);
			}
		}
	}
	END_FOR_STL(lineList);
	TRACE_DELETE_ARRAY(tempBuffer);
	// 解析valueList中的数据字符串
	std::map<std::string, std::string>::iterator iterValue = valueList.begin();
	std::map<std::string, std::string>::iterator iterValueEnd = valueList.end();
	FOR_STL(valueList, ; iterValue != iterValueEnd; ++iterValue)
	{
		if (floatParam)
		{
			std::map<std::string, SERVER_DEFINE_FLOAT>::iterator iterDefine = mFloatParamDefineList.find(iterValue->first);
			if (iterDefine != mFloatParamDefineList.end())
			{
				setParam(iterDefine->second, (float)atof(iterValue->second.c_str()));
			}
		}
		else
		{
			std::map<std::string, SERVER_DEFINE_STRING>::iterator iterDefine = mStringParamDefineList.find(iterValue->first);
			if (iterDefine != mStringParamDefineList.end())
			{
				setParam(iterDefine->second, iterValue->second.c_str());
			}
		}
	}
	END_FOR_STL(valueList);
}
