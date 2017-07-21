#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"
#include "CharacterData.h"
#include "MahjongAction.h"

void CommandCharacterMahjongEnd::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	txMap<CHAR_GUID, int> infoList;
	txMap<CharacterPlayer*, int>::iterator iter = mMoneyDeltaList.begin();
	txMap<CharacterPlayer*, int>::iterator iterEnd = mMoneyDeltaList.end();
	FOR_STL(mMoneyDeltaList, ; iter != iterEnd; ++iter)
	{
		infoList.insert(iter->first->getGUID(), iter->second);
	}
	END_FOR_STL(mMoneyDeltaList);
	SCNotifyMahjongEnd* mahjongEnd = static_cast<SCNotifyMahjongEnd*>(mNetManagerServer->createPacket(PT_SC_NOTIFY_MAHJONG_END));
	mahjongEnd->setList(infoList);
	mNetManagerServer->sendMessage(mahjongEnd, player->getClientGUID());
}

std::string CommandCharacterMahjongEnd::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}