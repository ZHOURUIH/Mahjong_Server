#ifndef _CS_LOGIN_H_
#define _CS_LOGIN_H_

#include "Packet.h"

const int ACCOUNT_LENGTH = 16;
const int PASSOWRD_LENGTH = 16;

class CSLogin : public Packet
{
public:
	CSLogin(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void execute();
	void setAccount(const std::string& account)
	{
		memcpy(mAccount, account.c_str(), account.length() < ACCOUNT_LENGTH ? account.length() : ACCOUNT_LENGTH);
	}
	void setPassword(const std::string& password)
	{
		memcpy(mPassword, password.c_str(), password.length() < PASSOWRD_LENGTH ? password.length() : PASSOWRD_LENGTH);
	}
	virtual void fillParams()
	{
		pushArrayParam(mAccount, ACCOUNT_LENGTH);
		pushArrayParam(mPassword, PASSOWRD_LENGTH);
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("account : %s, password : %s", mAccount, mPassword);
	}
public:
	char mAccount[ACCOUNT_LENGTH];
	char mPassword[PASSOWRD_LENGTH];
};

#endif