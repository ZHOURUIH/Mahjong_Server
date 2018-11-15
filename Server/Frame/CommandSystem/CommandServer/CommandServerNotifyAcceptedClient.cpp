#include "CommandHeader.h"
#include "NetServer.h"

void CommandServerNotifyAcceptedClient::execute()
{
	NetServer* gameServer = static_cast<NetServer*>(mReceiver);
	gameServer->notifyAcceptClient(mSocket, mIP.c_str());
}

string CommandServerNotifyAcceptedClient::showDebugInfo()
{
	COMMAND_DEBUG("socket : " + intToString(mSocket) + ", ip : " + mIP);
}