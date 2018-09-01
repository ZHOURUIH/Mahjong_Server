#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "Room.h"
#include "RoomManager.h"
#include "PacketHeader.h"
#include "NetServer.h"

void CommandCharacterMahjongPoolSize::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	// 发消息通知客户端
	SCMahjongPoolSize* mahjongPoolSize = NEW_PACKET(mahjongPoolSize, PT_SC_MAHJONG_POOL_SIZE);
	mahjongPoolSize->mCount = mCount;
	sendMessage(mahjongPoolSize, player);
}

std::string CommandCharacterMahjongPoolSize::showDebugInfo()
{
	COMMAND_DEBUG("mCount : %d", mCount);
}