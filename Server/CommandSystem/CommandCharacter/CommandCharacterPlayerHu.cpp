#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"
#include "MahjongAction.h"

void CommandCharacterPlayerHu::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCPlayerHu* hu = static_cast<SCPlayerHu*>(mNetServer->createPacket(PT_SC_PLAYER_HU));
	hu->setHuList(mHuList);
	mNetServer->sendMessage(hu, player->getClientGUID());
}

std::string CommandCharacterPlayerHu::showDebugInfo()
{
	int playerCount = mHuList.size();
	std::string info;
	for (int i = 0; i < playerCount; ++i)
	{
		info += "player : " + mHuList[i].first->getName() + "; ";
		int huCount = mHuList[i].second.size();
		for (int j = 0; j < huCount; ++j)
		{
			info += "hu list : " + txStringUtility::intToString((int)mHuList[i].second[j]);
		}
	}
	COMMAND_DEBUG("%s", info.c_str());
}