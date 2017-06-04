#include "PacketFactoryManager.h"
#include "PacketHeader.h"
#include "GameLog.h"

#define ADD_PACKET_FACTORY(packet, type) addFactory<PacketFactory<packet> >(type); addPacketSize(type);

std::map<PACKET_TYPE, int> PacketFactoryManager::mPacketSizeMap;

void PacketFactoryManager::init()
{
	// Client->Server
	ADD_PACKET_FACTORY(CSHeartBeat, PT_CS_HEART_BEAT);
	ADD_PACKET_FACTORY(CSLogin, PT_CS_LOGIN);
	ADD_PACKET_FACTORY(CSRegister, PT_CS_REGISTER);
	ADD_PACKET_FACTORY(CSCheckName, PT_CS_CHECK_NAME);
	ADD_PACKET_FACTORY(CSCheckAccount, PT_CS_CHECK_ACCOUNT);
	ADD_PACKET_FACTORY(CSCreateRoom, PT_CS_CREATE_ROOM);
	ADD_PACKET_FACTORY(CSJoinRoom, PT_CS_JOIN_ROOM);
	// Server->Client
	ADD_PACKET_FACTORY(SCHeartBeatRet, PT_SC_HEART_BEAT_RET);
	ADD_PACKET_FACTORY(SCLoginRet, PT_SC_LOGIN_RET);
	ADD_PACKET_FACTORY(SCOtherPlayerOffline, PT_SC_OTHER_PLAYER_OFFLINE);
	ADD_PACKET_FACTORY(SCStartGame, PT_SC_START_GAME);
	ADD_PACKET_FACTORY(SCRegisterRet, PT_SC_REGISTER_RET);
	ADD_PACKET_FACTORY(SCCheckNameRet, PT_SC_CHECK_NAME_RET);
	ADD_PACKET_FACTORY(SCCheckAccountRet, PT_SC_CHECK_ACCOUNT_RET);
	ADD_PACKET_FACTORY(SCCreateRoomRet, PT_SC_CREATE_ROOM_RET);
	ADD_PACKET_FACTORY(SCNotifyBanker, PT_SC_NOTIFY_BANKER);
	ADD_PACKET_FACTORY(SCOtherPlayerLeaveRoom, PT_SC_OTHER_PLAYER_LEAVE_ROOM);
	ADD_PACKET_FACTORY(SCOtherPlayerJoinRoom, PT_SC_OTHER_PLAYER_JOIN_ROOM);
	ADD_PACKET_FACTORY(SCJoinRoomRet, PT_SC_JOIN_ROOM_RET);
	int needCount = PT_CS_MAX - PT_CS_MIN - 1 + PT_SC_MAX - PT_SC_MIN - 1;
	if (mFactoryList.size() != needCount)
	{
		GAME_ERROR("error : packet factory not init success! factory count : %d, need count : %d", mFactoryList.size(), needCount);
	}
}