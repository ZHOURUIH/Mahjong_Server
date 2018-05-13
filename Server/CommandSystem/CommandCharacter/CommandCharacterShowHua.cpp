﻿#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterShowHua::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	player->showHua(mIndex);

	SCShowHua* hua = static_cast<SCShowHua*>(mNetServer->createPacket(PT_SC_SHOW_HUA));
	hua->mIndex = mIndex;
	hua->mMahjong = mMahjong;
	mNetServer->sendMessage(hua, player->getClientGUID());
}

std::string CommandCharacterShowHua::showDebugInfo()
{
	COMMAND_DEBUG("index : %d, mahjong : %s", mIndex, MAHJONG_NAME[(int)mMahjong].c_str());
}