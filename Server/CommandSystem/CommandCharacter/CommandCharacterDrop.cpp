#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"
#include "CharacterData.h"

void CommandCharacterDrop::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	player->dropMahjong(mIndex);

	SCRequestDropRet* requestDropRet = static_cast<SCRequestDropRet*>(mNetManagerServer->createPacket(PT_SC_REQUEST_DROP_RET));
	requestDropRet->mIndex = mIndex;
	requestDropRet->mMahjong = mMahjong;
	mNetManagerServer->sendMessage(requestDropRet, player->getClientGUID());

	// 打出一张牌后,需要重新排列
	CommandCharacterReorderMahjong cmdReorder(CMD_PARAM);
	mCommandSystem->pushCommand(&cmdReorder, player);
}

std::string CommandCharacterDrop::showDebugInfo()
{
	COMMAND_DEBUG("index : %d, mahjong : %s", mIndex, MAHJONG_NAME[mMahjong].c_str());
}