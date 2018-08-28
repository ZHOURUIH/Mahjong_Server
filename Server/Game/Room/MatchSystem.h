#ifndef _MATCH_SYSTEM_H_
#define _MATCH_SYSTEM_H_

#include "ServerDefine.h"
#include "FrameComponent.h"

class CharacterPlayer;
class MatchInfo
{
public:
	MatchInfo()
	{
		mPlayer = NULL;
		mWaitTime = 0.0f;
	}
public:
	CharacterPlayer* mPlayer;
	float mWaitTime;
};

class MatchSystem : public FrameComponent
{
public:
	MatchSystem(const std::string& name)
		:FrameComponent(name) {}
	virtual void init();
	virtual void update(float elapsedTime);
	void requestFreeMatch(CharacterPlayer* player);
	void notifyPlayerOffline(CharacterPlayer* player);
protected:
	txMap<CHAR_GUID, MatchInfo*> mFreeMatchPoolMap;	// 等待匹配的玩家列表,用于查找
	txVector<MatchInfo*>	mFreeMatchPool;				// 等待匹配的玩家列表,保存着玩家之间的先后顺序
};

#endif