#include "PacketHeader.h"
#ifdef _SERVER
#include "NetManagerServer.h"
#include "NetManagerClient.h"
#endif

void CSHeartBeat::execute()
{
#ifdef _SERVER
	NetManagerClient* client = mNetManagerServer->getClient(mClient);
	if (client != NULL)
	{
		client->notifyReceiveHeartBeat();
	}
	SCHeartBeatRet* heartBeat = static_cast<SCHeartBeatRet*>(mNetManagerServer->createPacket(PT_SC_HEART_BEAT_RET));
	heartBeat->mHeartBeatTimes = mHeartBeatTimes;
	mNetManagerServer->sendMessage(heartBeat, mClient);
#endif
}
