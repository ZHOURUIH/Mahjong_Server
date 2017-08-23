﻿#ifndef _CHARACTER_PLAYER_H_
#define _CHARACTER_PLAYER_H_

#include "CharacterNPC.h"
#include "GameDefine.h"

class CharacterPlayer : public CharacterNPC
{
public:
	CharacterPlayer(const CHARACTER_TYPE& type, const std::string& name)
		:
		CharacterNPC(type, name),
		mClientGUID(INVALID_ID)
	{}
	virtual ~CharacterPlayer(){ destroy(); }
	void destroy();
	void setClientGUID(const CLIENT_GUID& client) { mClientGUID = client; }
	const CLIENT_GUID& getClientGUID() { return mClientGUID; }
	void reorderMahjong();
	void getMahjong(const MAHJONG& mah);
	void showHua(const int& index);
	void dropMahjong(const int& index);
	void gangMahjong(const MAHJONG& mahjong, CharacterPlayer* dropPlayer);
	void pengMahjong(const MAHJONG& mahjong);
	void clearMahjong();
protected:
	void addPeng(const MAHJONG& mahjong);
	void addGang(const MAHJONG& mahjong);
	void pengToGang(const MAHJONG& mahjong);
	bool hasPeng(const MAHJONG& mahjong, int& pengIndex);
protected:
	CLIENT_GUID mClientGUID;	// 玩家所在客户端的GUID
};

#endif
