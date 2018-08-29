#include "PacketHeader.h"
#include "NetServer.h"
#include "NetClient.h"

void CSHeartBeat::execute()
{
	SCHeartBeatRet* heartBeat = NEW_PACKET(heartBeat, PT_SC_HEART_BEAT_RET);
	heartBeat->mHeartBeatTimes = mHeartBeatTimes;
	sendMessage(heartBeat, mClient);
}
