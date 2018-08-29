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
	// 先向客户端发回是否继续游戏的结果
	if (mContinue)
	{
		SCContinueGameRet* continueRet = NEW_PACKET(continueRet, PT_SC_CONTINUE_GAME_RET);
		continueRet->mBanker = player->getCharacterData()->mBanker;
		sendMessage(continueRet, player);
	}
	else
	{
		SCBackToMahjongHallRet* backRet = NEW_PACKET(backRet, PT_SC_BACK_TO_MAHJONG_HALL_RET);
		sendMessage(backRet, player);
	}

	// 通知房间有玩家选择是否继续游戏
	Room* curRoom = mRoomManager->getRoom(player->getCharacterData()->mRoomID);
	CommandRoomNotifyPlayerContinueGame* cmdPlayerContinue = NEW_CMD_INFO(cmdPlayerContinue);
	cmdPlayerContinue->mPlayer = player;
	cmdPlayerContinue->mContinue = mContinue;
	pushCommand(cmdPlayerContinue, curRoom);
}