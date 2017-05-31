#ifndef _COMMAND_SERVER_NOTIFY_ACCEPTED_CLIENT_H_
#define _COMMAND_SERVER_NOTIFY_ACCEPTED_CLIENT_H_

#include "txCommand.h"

class CommandServerNotifyAcceptedClient : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandServerNotifyAcceptedClient)
		, mSocket(INVALID_ID)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	SOCKET mSocket;
	std::string mIP;
};

#endif