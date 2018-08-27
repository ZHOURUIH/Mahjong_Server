#ifndef _CHARACTER_PLAYER_H_
#define _CHARACTER_PLAYER_H_

#include "CharacterNPC.h"
#include "GameDefine.h"

class MahjongAction;
class CharacterPlayer : public Character
{
public:
	CharacterPlayer(CHARACTER_TYPE type, const std::string& name)
		:
		Character(type, name),
		mClientGUID(INVALID_ID)
	{}
	virtual ~CharacterPlayer(){ destroy(); }
	void destroy();
	void setClientGUID(CLIENT_GUID client) { mClientGUID = client; }
	CLIENT_GUID getClientGUID() { return mClientGUID; }
	void reorderMahjong();
	void getMahjong(MAHJONG mah);
	void CharacterPlayer::getHua(MAHJONG hua);
	void dropMahjong(int index);
	void gangMahjong(MAHJONG mahjong, CharacterPlayer* dropPlayer);
	void pengMahjong(MAHJONG mahjong);
	void clearMahjong();
	// 重载函数
	virtual void notifyStartGame() {}
	virtual void notifyAskAction(txVector<MahjongAction*>& actionList) {}
	virtual void notifyAskDrop() {}
protected:
	void addPeng(MAHJONG mahjong);
	void addGang(MAHJONG mahjong);
	bool hasPeng(MAHJONG mahjong, int& pengIndex);
protected:
	CLIENT_GUID mClientGUID;	// 玩家所在客户端的GUID
};

#endif
