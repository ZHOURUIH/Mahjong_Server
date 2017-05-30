#ifndef _SC_LOGIN_RET_H_
#define _SC_LOGIN_RET_H_

#include "Packet.h"

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
		pushParam(mGUID, "guid");
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("login ret : %d, guid : %d", mLoginRet, mGUID);
	}
public:
	char mLoginRet;			// 登录结果,1为成功,0为失败,-1表示已在其他地方登陆
	int mGUID;
};

#endif