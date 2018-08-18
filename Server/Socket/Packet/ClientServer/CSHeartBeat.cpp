#include "PacketHeader.h"
#ifdef _SERVER
#include "NetServer.h"
#include "NetClient.h"
#endif

void CSHeartBeat::execute()
{
#ifdef _SERVER
	NetClient* client = mNetServer->getClient(mClient);
	if (client != NULL)
	{
		client->notifyReceiveHeartBeat();
	}
	SCHeartBeatRet* heartBeat = NetServer::createPacket(heartBeat, PT_SC_HEART_BEAT_RET);
	heartBeat->mHeartBeatTimes = mHeartBeatTimes;
	mNetServer->sendMessage(heartBeat, mClient);
#endif
}
