#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"
#include "CharacterMahjongRobot.h"

void CommandCharacterStartGame::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	// 取消玩家的准备标记
	player->getCharacterData()->mReady = false;
	// 通知玩家游戏开始
	player->notifyStartGame();
	// 如果是真实玩家,则需要发送消息到客户端
	if (player->getType() == CT_PLAYER)
	{
		// 发送开局麻将的信息
		SCStartGame* startGamePacket = NEW_PACKET(startGamePacket, PT_SC_START_GAME);
		startGamePacket->setDice(mDice[0], mDice[1]);
		startGamePacket->setMahjongList(mPlayerIDList, mHandInList, mHuaList);
		sendMessage(startGamePacket, player);
	}
}

std::string CommandCharacterStartGame::showDebugInfo()
{
	COMMAND_DEBUG("dice : %d, %d", (int)mDice[0], (int)mDice[1]);
}