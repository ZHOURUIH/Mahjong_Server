#ifndef _FRAME_CONFIG_H_
#define _FRAME_CONFIG_H_

#include "FrameComponent.h"

class FrameConfig : public FrameComponent
{
public:
	FrameConfig(const string& name);
	virtual ~FrameConfig(){ destory(); }
	virtual void init();
	void destory()
	{
		mFloatParamDefineList.clear();
		mStringParamDefineList.clear();
		mFloatParamList.clear();
		mStringParamList.clear();
	}
	float getFloatParam(FRAME_DEFINE_FLOAT param)
	{
		return mFloatParamList.tryGet(param, 0.0f);
	}
	const string& getStringParam(FRAME_DEFINE_STRING param)
	{
		return mStringParamList.tryGet(param, EMPTY_STRING);
	}
protected:
	void setParam(FRAME_DEFINE_FLOAT param, float value)
	{
		mFloatParamList.trySet(param, value);
	}
	void setParam(FRAME_DEFINE_STRING param, const string& value)
	{
		mStringParamList.trySet(param, value);
	}
	virtual void readConfig(txMap<string, string>& valueList, bool floatParam);
	static void parseConfig(FrameConfig* config, const string& fileName, bool floatParam);
protected:
	txMap<string, FRAME_DEFINE_FLOAT> mFloatParamDefineList;
	txMap<string, FRAME_DEFINE_STRING> mStringParamDefineList;
	txMap<FRAME_DEFINE_FLOAT, float> mFloatParamList;
	txMap<FRAME_DEFINE_STRING, string> mStringParamList;
};

#endif