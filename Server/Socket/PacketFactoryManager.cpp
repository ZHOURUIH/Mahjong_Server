#include "PacketFactoryManager.h"
#include "PacketHeader.h"
#include "GameLog.h"

#define ADD_PACKET_FACTORY(packet, type) addFactory<PacketFactory<packet> >(type); addPacketSize(type);

std::map<PACKET_TYPE, int> PacketFactoryManager::mPacketSizeMap;

void PacketFactoryManager::init()
{
	ADD_PACKET_FACTORY(CSHeartBeat, PT_CS_HEART_BEAT);
	ADD_PACKET_FACTORY(CSLogin, PT_CS_LOGIN);
	ADD_PACKET_FACTORY(CSRegister, PT_CS_REGISTER);
	ADD_PACKET_FACTORY(CSCheckName, PT_CS_CHECK_NAME);
	ADD_PACKET_FACTORY(CSCheckAccount, PT_CS_CHECK_ACCOUNT);
	ADD_PACKET_FACTORY(SCHeartBeatRet, PT_SC_HEART_BEAT_RET);
	ADD_PACKET_FACTORY(SCLoginRet, PT_SC_LOGIN_RET);
	ADD_PACKET_FACTORY(SCPlayerOffline, PT_SC_PLAYER_OFFLINE);
	ADD_PACKET_FACTORY(SCStartGame, PT_SC_START_GAME);
	ADD_PACKET_FACTORY(SCRegisterRet, PT_SC_REGISTER_RET);
	ADD_PACKET_FACTORY(SCCheckNameRet, PT_SC_CHECK_NAME);
	ADD_PACKET_FACTORY(SCCheckAccountRet, PT_SC_CHECK_ACCOUNT);
	int actualCount = PT_CS_MAX - PT_CS_MIN - 1 + PT_SC_MAX - PT_SC_MIN - 1;
	if (mFactoryList.size() != actualCount)
	{
		GAME_ERROR("error : packet factory not init success! factory count : %d, packet type count : %d", mFactoryList.size(), actualCount);
	}
}