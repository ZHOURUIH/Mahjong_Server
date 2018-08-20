#ifndef _SERVER_CONFIG_H_
#define _SERVER_CONFIG_H_

#include "FrameComponent.h"

class ServerConfig : public FrameComponent
{
public:
	ServerConfig(const std::string& name);
	virtual ~ServerConfig(){ destory(); }
	virtual void init();
	void readConfig(const std::string& fileName, bool floatParam);
	void destory()
	{
		mFloatParamDefineList.clear();
		mStringParamDefineList.clear();
		mFloatParamList.clear();
		mStringParamList.clear();
	}
	static float getFloatParam(SERVER_DEFINE_FLOAT param)
	{
		return mFloatParamList.tryGet(param, 0.0f);
	}
	static const std::string& getStringParam(SERVER_DEFINE_STRING param)
	{
		return mStringParamList.tryGet(param, EMPTY_STRING);
	}
protected:
	void setParam(SERVER_DEFINE_FLOAT param, float value)
	{
		mFloatParamList.trySet(param, value);
	}
	void setParam(SERVER_DEFINE_STRING param, const std::string& value)
	{
		mStringParamList.trySet(param, value);
	}
protected:
	static txMap<std::string, SERVER_DEFINE_FLOAT> mFloatParamDefineList;
	static txMap<std::string, SERVER_DEFINE_STRING> mStringParamDefineList;
	static txMap<SERVER_DEFINE_FLOAT, float> mFloatParamList;
	static txMap<SERVER_DEFINE_STRING, std::string> mStringParamList;
};

#endif