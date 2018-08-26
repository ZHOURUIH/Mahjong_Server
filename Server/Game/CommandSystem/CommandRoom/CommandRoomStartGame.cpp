#include "CommandHeader.h"
#include "Room.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "Utility.h"

void CommandRoomStartGame::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	int dice0 = MathUtility::randomInt(0, MAX_DICE - 1);
	int dice1 = MathUtility::randomInt(0, MAX_DICE - 1);
	txVector<txVector<MAHJONG>> handInList;
	txVector<txVector<MAHJONG>> huaList;
	txVector<CHAR_GUID> playerIDList;
	room->generateStartMahjong(handInList, huaList);
	auto& playerList = room->getPlayerList();
	auto iterPlayer = playerList.begin();
	auto iterPlayerEnd = playerList.end();
	// 先获取房间内玩家ID列表
	FOR_STL(playerList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		playerIDList.push_back(iterPlayer->second->getCharacterData()->mGUID);
	}
	END_FOR_STL(playerList);
	FOR_STL (playerList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		// 取消玩家的准备标记
		iterPlayer->second->getCharacterData()->mReady = false;
		// 通知玩家开始游戏
		CommandCharacterStartGame* cmdStartGame = NEW_CMD_INFO(cmdStartGame);
		cmdStartGame->setDice(dice0, dice1);
		cmdStartGame->mHandInList = handInList;
		cmdStartGame->mHuaList = huaList;
		cmdStartGame->mPlayerIDList = playerIDList;
		mCommandSystem->pushCommand(cmdStartGame, iterPlayer->second);
	}
	END_FOR_STL(playerList);
}

std::string CommandRoomStartGame::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}