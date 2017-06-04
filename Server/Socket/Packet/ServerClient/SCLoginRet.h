#ifndef _SC_LOGIN_RET_H_
#define _SC_LOGIN_RET_H_

#include "Packet.h"

const int NAME_LENGTH = 16;

class SCLoginRet : public Packet
{
public:
	SCLoginRet(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void execute(){}
	virtual void fillParams()
	{
		pushParam(mLoginRet, "login ret");
		pushArrayParam(mName, NAME_LENGTH, "login ret");
		pushParam(mMoney, "money");
		pushParam(mHead, "head");
		pushParam(mGUID, "guid");
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("login ret : %d, guid : %d, name : %s, money : %d, head : %d", mLoginRet, mGUID, mName, mMoney, mHead);
	}
	void setName(const std::string& name)
	{
		memcpy(mName, name.c_str(), name.length() < NAME_LENGTH ? name.length() : NAME_LENGTH);
	}
public:
	char mLoginRet;			// 0表示登陆成功,1表示账号密码错误,2表示已经在其他地方登陆
	char mName[NAME_LENGTH];
	int mMoney;
	short mHead;
	int mGUID;
};

#endif