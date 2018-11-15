#include "Utility.h"
#include "FrameConfig.h"
#include "GameLog.h"

#define ADD_FLOAT_PARAM(t) mFloatParamDefineList.insert(TOSTRING(t), t);
#define ADD_STRING_PARAM(t) mStringParamDefineList.insert(TOSTRING(t), t);

FrameConfig::FrameConfig(const string& name)
	:FrameComponent(name)
{
	ADD_FLOAT_PARAM(FDF_SOCKET_PORT);
	ADD_FLOAT_PARAM(FDF_HEART_BEAT_TIME_OUT);
	ADD_FLOAT_PARAM(FDF_BACK_LOG);
	ADD_FLOAT_PARAM(FDF_SHOW_COMMAND_DEBUG_INFO);
	ADD_FLOAT_PARAM(FDF_OUTPUT_NET_LOG);
	if (mFloatParamDefineList.size() != FDF_MAX)
	{
		LOG_ERROR("not all float parameters init! init count : "+ intToString(mFloatParamDefineList.size())+ " , max count : " + intToString(FDF_MAX));
	}

	ADD_STRING_PARAM(FDS_DOMAIN_NAME);
	if (mStringParamDefineList.size() != FDS_MAX)
	{
		LOG_ERROR("not all string parameters init! init count : " + intToString(mStringParamDefineList.size()) + ", max count : " + intToString(FDS_MAX));
	}
}

void FrameConfig::init()
{
	parseConfig(this, CONFIG_PATH + "FrameFloatConfig.txt", true);
	parseConfig(this, CONFIG_PATH + "FrameStringConfig.txt", false);
}

void FrameConfig::readConfig(txMap<string, string>& valueList, bool floatParam)
{
	// 解析valueList中的数据字符串
	auto iterValue = valueList.begin();
	auto iterValueEnd = valueList.end();
	for (; iterValue != iterValueEnd; ++iterValue)
	{
		if (floatParam)
		{
			auto iterDefine = mFloatParamDefineList.find(iterValue->first);
			if (iterDefine != mFloatParamDefineList.end())
			{
				setParam(iterDefine->second, (float)atof(iterValue->second.c_str()));
			}
		}
		else
		{
			auto iterDefine = mStringParamDefineList.find(iterValue->first);
			if (iterDefine != mStringParamDefineList.end())
			{
				setParam(iterDefine->second, iterValue->second.c_str());
			}
		}
	}
}

void FrameConfig::parseConfig(FrameConfig* config, const string& fileName, bool floatParam)
{
	string dataString = openTxtFile(getAvailableResourcePath(fileName));
#if RUN_PLATFORM == PLATFORM_WINDOWS
	const char* returnNextLine = "\r\n";
#elif RUN_PLATFORM == PLATFORM_LINUX
	const char* returnNextLine = "\n";
#endif
	// 将文件的每一行处理后放入lineList中
	txVector<string> lineList;
	split(dataString, returnNextLine, lineList);

	// 每行最多只允许2048个字节
	const int LINE_MAX_LENGTH = 2048;
	char* tempBuffer = TRACE_NEW_ARRAY(char, LINE_MAX_LENGTH, tempBuffer);
	// 将数据字符串拆分出来,放入valueList中
	txMap<string, string> valueList;
	int lineCount = lineList.size();
	for(int i = 0; i < lineCount; ++i)
	{
		const string& lineString = lineList[i];
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
		string newString(tempBuffer);
		// 如果该行是空的,或者是注释,则不进行处理
		if (newString.length() > 0 && !startWith(newString, "//"))
		{
			txVector<string> valueVector;
			split(newString, "=", valueVector);
			if (valueVector.size() == 2)
			{
				valueList.insert(valueVector[0], valueVector[1]);
			}
		}
	}
	TRACE_DELETE_ARRAY(tempBuffer);
	config->readConfig(valueList, floatParam);
}
