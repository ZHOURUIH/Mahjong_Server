#ifndef _SERVER_CONFIG_H_
#define _SERVER_CONFIG_H_

#include "ServerDefine.h"
#include "ServerBase.h"

class ServerConfig : public ServerBase
{
public:
	ServerConfig();
	virtual ~ServerConfig(){ destory(); }
	void init();
	void readConfig(const std::string& fileName, const bool& floatParam);
	void destory()
	{
		mFloatParamDefineList.clear();
		mStringParamDefineList.clear();
		mFloatParamList.clear();
		mStringParamList.clear();
	}
	static float getFloatParam(const SERVER_DEFINE_FLOAT& param)
	{
		txMap<SERVER_DEFINE_FLOAT, float>::iterator iterParam = mFloatParamList.find(param);
		if (iterParam != mFloatParamList.end())
		{
			return iterParam->second;
		}
		return 0.0f;
	}
	static const std::string& getStringParam(const SERVER_DEFINE_STRING& param)
	{
		std::map<SERVER_DEFINE_STRING, std::string>::iterator iterParam = mStringParamList.find(param);
		if (iterParam != mStringParamList.end())
		{
			return iterParam->second;
		}
		return EMPTY_STRING;
	}
protected:
	void setParam(const SERVER_DEFINE_FLOAT& param, const float& value)
	{
		txMap<SERVER_DEFINE_FLOAT, float>::iterator iterParam = mFloatParamList.find(param);
		if (iterParam == mFloatParamList.end())
		{
			mFloatParamList.insert(param, value);
		}
		else
		{
			iterParam->second = value;
		}
	}
	void setParam(const SERVER_DEFINE_STRING& param, const std::string& value)
	{
		txMap<SERVER_DEFINE_STRING, std::string>::iterator iterParam = mStringParamList.find(param);
		if (iterParam == mStringParamList.end())
		{
			mStringParamList.insert(param, value);
		}
		else
		{
			iterParam->second = value;
		}
	}
protected:
	static txMap<std::string, SERVER_DEFINE_FLOAT> mFloatParamDefineList;
	static txMap<std::string, SERVER_DEFINE_STRING> mStringParamDefineList;
	static txMap<SERVER_DEFINE_FLOAT, float> mFloatParamList;
	static txMap<SERVER_DEFINE_STRING, std::string> mStringParamList;
};

#endif