#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"
#include "CharacterData.h"
#include "MahjongAction.h"

void CommandCharacterMahjongEnd::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	std::map<CHAR_GUID, int> infoList;
	std::map<CharacterPlayer*, int>::iterator iter = mMoneyDeltaList.begin();
	std::map<CharacterPlayer*, int>::iterator iterEnd = mMoneyDeltaList.end();
	for (; iter != iterEnd; ++iter)
	{
		infoList.insert(std::make_pair(iter->first->getGUID(), iter->second));
	}
	SCNotifyMahjongEnd* mahjongEnd = static_cast<SCNotifyMahjongEnd*>(mNetManagerServer->createPacket(PT_SC_NOTIFY_MAHJONG_END));
	mahjongEnd->setList(infoList);
	mNetManagerServer->sendMessage(mahjongEnd, player->getClientGUID());
}

std::string CommandCharacterMahjongEnd::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}