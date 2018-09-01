#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "RoomManager.h"
#include "Room.h"
#include "CharacterData.h"
#include "NetServer.h"
#include "PacketHeader.h"

void CommandCharacterContinueGame::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	// 先向客户端发回继续游戏的结果
	SCContinueGameRet* continueRet = NEW_PACKET(continueRet, PT_SC_CONTINUE_GAME_RET);
	continueRet->mBanker = player->getCharacterData()->mBanker;
	sendMessage(continueRet, player);
}