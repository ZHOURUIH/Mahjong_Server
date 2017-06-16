#ifndef _CHARACTER_PLAYER_H_
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
	void destroy(){}
	void setClientGUID(const CLIENT_GUID& client) { mClientGUID = client; }
	const CLIENT_GUID& getClientGUID() { return mClientGUID; }
	void reorderMahjong();
	void getMahjongStart(MAHJONG mah);
protected:
	CLIENT_GUID mClientGUID;	// 玩家所在客户端的GUID
};

#endif
