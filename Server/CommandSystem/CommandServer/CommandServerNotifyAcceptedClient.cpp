#include "CommandHeader.h"
#include "NetManagerServer.h"

void CommandServerNotifyAcceptedClient::execute()
{
	NetManagerServer* gameServer = static_cast<NetManagerServer*>(mReceiver);
	gameServer->notifyAcceptClient(mSocket, mIP.c_str());
}

std::string CommandServerNotifyAcceptedClient::showDebugInfo()
{
	COMMAND_DEBUG("socket : %d, ip : %s", mSocket, mIP.c_str());
}