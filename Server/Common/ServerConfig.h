#ifndef _SERVER_CONFIG_H_
#define _SERVER_CONFIG_H_

#include "ServerDefine.h"
#include "ServerBase.h"

class ServerConfig : public ServerBase
{
public:
	ServerConfig();
	virtual ~ServerConfig(){ destory(); }
	void init(){ readConfig(CONFIG_PATH + "GameFloatConfig.txt"); }
	void readConfig(const std::string& fileName);
	void destory()
	{
		mFloatParamDefineList.clear();
		mFloatParamList.clear();
	}
	static float getFloatParam(const SERVER_DEFINE& param)
	{
		std::map<SERVER_DEFINE, float>::iterator iterParam = mFloatParamList.find(param);
		if (iterParam != mFloatParamList.end())
		{
			return iterParam->second;
		}
		return 0.0f;
	}
protected:
	void setFloatParam(const SERVER_DEFINE& param, const float& value)
	{
		std::map<SERVER_DEFINE, float>::iterator iterParam = mFloatParamList.find(param);
		if (iterParam == mFloatParamList.end())
		{
			mFloatParamList.insert(std::make_pair(param, value));
		}
		else
		{
			iterParam->second = value;
		}
	}
protected:
	static std::map<std::string, SERVER_DEFINE> mFloatParamDefineList;
	static std::map<SERVER_DEFINE, float> mFloatParamList;
};

#endif