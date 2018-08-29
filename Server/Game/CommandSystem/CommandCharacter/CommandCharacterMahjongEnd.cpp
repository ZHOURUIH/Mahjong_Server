#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"
#include "MahjongAction.h"

void CommandCharacterMahjongEnd::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	txMap<CHAR_GUID, int> infoList;
	auto iter = mMoneyDeltaList.begin();
	auto iterEnd = mMoneyDeltaList.end();
	FOR(mMoneyDeltaList, ; iter != iterEnd; ++iter)
	{
		infoList.insert(iter->first->getGUID(), iter->second);
	}
	END(mMoneyDeltaList);
	SCNotifyMahjongEnd* mahjongEnd = NEW_PACKET(mahjongEnd, PT_SC_NOTIFY_MAHJONG_END);
	mahjongEnd->setList(infoList);
	sendMessage(mahjongEnd, player);
}