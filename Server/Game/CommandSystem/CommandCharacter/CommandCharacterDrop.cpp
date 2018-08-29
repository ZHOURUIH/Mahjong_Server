#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterDrop::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	player->dropMahjong(mIndex);

	if (player->getType() == CT_PLAYER)
	{
		SCRequestDropRet* requestDropRet = NEW_PACKET(requestDropRet, PT_SC_REQUEST_DROP_RET);
		requestDropRet->mIndex = mIndex;
		requestDropRet->mMahjong = mMahjong;
		sendMessage(requestDropRet, player);
	}

	// 打出一张牌后,需要重新排列
	CommandCharacterReorderMahjong* cmd = NEW_CMD_INFO(cmd);
	pushCommand(cmd, player);
}

std::string CommandCharacterDrop::showDebugInfo()
{
	COMMAND_DEBUG("index : %d, mahjong : %s", mIndex, MAHJONG_NAME[(int)mMahjong].c_str());
}