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
	txMap<CharacterPlayer*, int>::iterator iter = mMoneyDeltaList.begin();
	txMap<CharacterPlayer*, int>::iterator iterEnd = mMoneyDeltaList.end();
	FOR_STL(mMoneyDeltaList, ; iter != iterEnd; ++iter)
	{
		infoList.insert(iter->first->getGUID(), iter->second);
	}
	END_FOR_STL(mMoneyDeltaList);
	SCNotifyMahjongEnd* mahjongEnd = static_cast<SCNotifyMahjongEnd*>(mNetServer->createPacket(PT_SC_NOTIFY_MAHJONG_END));
	mahjongEnd->setList(infoList);
	mNetServer->sendMessage(mahjongEnd, player->getClientGUID());
}