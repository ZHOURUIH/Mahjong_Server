﻿#ifndef _SERVER_CONFIG_H_
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
		auto iterParam = mFloatParamList.find(param);
		if (iterParam != mFloatParamList.end())
		{
			return iterParam->second;
		}
		return 0.0f;
	}
	static const std::string& getStringParam(SERVER_DEFINE_STRING param)
	{
		auto iterParam = mStringParamList.find(param);
		if (iterParam != mStringParamList.end())
		{
			return iterParam->second;
		}
		return EMPTY_STRING;
	}
protected:
	void setParam(SERVER_DEFINE_FLOAT param, float value)
	{
		auto iterParam = mFloatParamList.find(param);
		if (iterParam == mFloatParamList.end())
		{
			mFloatParamList.insert(param, value);
		}
		else
		{
			iterParam->second = value;
		}
	}
	void setParam(SERVER_DEFINE_STRING param, const std::string& value)
	{
		auto iterParam = mStringParamList.find(param);
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