#ifndef _CHARACTER_PLAYER_H_
#define _CHARACTER_PLAYER_H_

#include "CharacterNPC.h"

class CharacterPlayer : public CharacterNPC
{
public:
	CharacterPlayer(const CHARACTER_TYPE& type, const std::string& name)
		:
		CharacterNPC(type, name),
		mClientGUID(~0)
	{}
	virtual ~CharacterPlayer(){ destroy(); }
	void destroy(){}
	void setClientGUID(const CLIENT_GUID& client) { mClientGUID = client; }
	const CLIENT_GUID& getClientGUID() { return mClientGUID; }
protected:
	CLIENT_GUID mClientGUID;	// 玩家的客户端GUID
};

#endif
