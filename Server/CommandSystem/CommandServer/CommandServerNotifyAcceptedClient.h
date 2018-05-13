﻿#ifndef _COMMAND_SERVER_NOTIFY_ACCEPTED_CLIENT_H_
#define _COMMAND_SERVER_NOTIFY_ACCEPTED_CLIENT_H_

#include "txCommand.h"

class CommandServerNotifyAcceptedClient : public txCommand
{
public:
	virtual void reset()
	{
		mSocket = INVALID_ID;
		mIP = EMPTY_STRING;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	TX_SOCKET mSocket;
	std::string mIP;
};

#endif