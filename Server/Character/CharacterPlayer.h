#ifndef _CHARACTER_PLAYER_H_
#define _CHARACTER_PLAYER_H_

#include "CharacterNPC.h"
#include "GameDefine.h"

class CharacterPlayer : public CharacterNPC
{
public:
	CharacterPlayer(CHARACTER_TYPE type, const std::string& name)
		:
		CharacterNPC(type, name),
		mClientGUID(INVALID_ID)
	{}
	virtual ~CharacterPlayer(){ destroy(); }
	void destroy();
	void setClientGUID(CLIENT_GUID client) { mClientGUID = client; }
	CLIENT_GUID getClientGUID() { return mClientGUID; }
	void reorderMahjong();
	void getMahjong(MAHJONG mah);
	void showHua(int index);
	void dropMahjong(int index);
	void gangMahjong(MAHJONG mahjong, CharacterPlayer* dropPlayer);
	void pengMahjong(MAHJONG mahjong);
	void clearMahjong();
protected:
	void addPeng(MAHJONG mahjong);
	void addGang(MAHJONG mahjong);
	void pengToGang(MAHJONG mahjong);
	bool hasPeng(MAHJONG mahjong, int& pengIndex);
protected:
	CLIENT_GUID mClientGUID;	// 玩家所在客户端的GUID
};

#endif
