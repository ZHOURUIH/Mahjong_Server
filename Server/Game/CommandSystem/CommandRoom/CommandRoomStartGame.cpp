#include "CommandHeader.h"
#include "Room.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "Utility.h"
#include "CharacterMahjongRobot.h"

void CommandRoomStartGame::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	int dice0 = MathUtility::randomInt(0, MAX_DICE - 1);
	int dice1 = MathUtility::randomInt(0, MAX_DICE - 1);
	txVector<txVector<MAHJONG>> handInList;
	txVector<txVector<MAHJONG>> huaList;
	txVector<CHAR_GUID> playerIDList;
	room->setMahjongState(MPS_GET_START);
	room->generateStartMahjong(playerIDList, handInList, huaList);

	// 玩家拿到开局的麻将
	int playerCount = playerIDList.size();
	FOR(playerIDList, int i = 0; i < playerCount; ++i)
	{
		CharacterPlayer* player = static_cast<CharacterPlayer*>(room->getMember(playerIDList[i]));
		// 手里的麻将
		int mahjongCount = handInList[i].size();
		FOR(handInList[i], int j = 0; j < mahjongCount; ++j)
		{
			player->getMahjong(handInList[i][j]);
		}
		END(handInList[i]);
		// 花牌
		int huaCount = huaList[i].size();
		FOR(huaList[i], int j = 0; j < huaCount; ++j)
		{
			player->getHua(huaList[i][j]);
		}
		END(huaList[i]);
	}
	END(playerIDList);

	// 通知所有玩家开始游戏
	auto& playerList = room->getPlayerList();
	auto iterPlayer = playerList.begin();
	auto iterPlayerEnd = playerList.end();
	FOR_R(playerList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{	
		CommandCharacterStartGame* cmdStartGame = NEW_CMD_INFO(cmdStartGame);
		cmdStartGame->setDice(dice0, dice1);
		cmdStartGame->mHandInList = handInList;
		cmdStartGame->mHuaList = huaList;
		cmdStartGame->mPlayerIDList = playerIDList;
		pushCommand(cmdStartGame, iterPlayer->second);
	}
	END_R(playerList);
}