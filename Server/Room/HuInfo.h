#ifndef _HU_INFO_H_
#define _HU_INFO_H_

#include "GameDefine.h"

class CharacterPlayer;
class HuInfo
{
public:
	CharacterPlayer* mHuPlayer;			// ���Ƶ����
	CharacterPlayer* mDroppedPlayer;	// ��������Ƶ����,���Ϊ����,��Ϊ��
	MAHJONG mMahjong;					// ���Ƶ��齫
	std::vector<HU_TYPE> mHuList;		// ���Ʒ����б�
};

#endif