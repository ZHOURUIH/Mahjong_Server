#ifndef _WAIT_ACTION_INFO_H_
#define _WAIT_ACTION_INFO_H_

#include "GameDefine.h"

class CharacterPlayer;
class MahjongAction;
class WaitActionInfo
{
public:
	CharacterPlayer* mPlayer;				// �ȴ�ȷ�ϲ��������
	CharacterPlayer* mDroppedPlayer;		// ��������Ƶ����
	txVector<MahjongAction*> mActionList;// ��ҿ�ѡ��Ĳ���
	MAHJONG mMahjong;						// ��ǰ�齫
	MahjongAction* mConfirmedAction;		// ���ȷ��ѡ���һ�ֲ���
	WaitActionInfo(CharacterPlayer* player, CharacterPlayer* droppedPlayer, const txVector<MahjongAction*>& actionList, const MAHJONG& mah)
	{
		mPlayer = player;
		mDroppedPlayer = droppedPlayer;
		mActionList = actionList;
		mMahjong = mah;
		mConfirmedAction = NULL;
	}
};

#endif