#include "CommandHeaderBase.h"
#include "NetServer.h"

void CommandServerNotifyAcceptedClient::execute()
{
	NetServer* gameServer = static_cast<NetServer*>(mReceiver);
	gameServer->notifyAcceptClient(mSocket, mIP.c_str());
}

std::string CommandServerNotifyAcceptedClient::showDebugInfo()
{
	COMMAND_DEBUG("socket : %d, ip : %s", mSocket, mIP.c_str());
}