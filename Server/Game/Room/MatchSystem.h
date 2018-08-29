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
	txVector<MatchInfo*>& getFreeMatchPool() { return mFreeMatchPool; }
protected:
	txMap<CHAR_GUID, MatchInfo*> mFreeMatchPoolMap;	// �ȴ�ƥ�������б�,���ڲ���
	txVector<MatchInfo*>	mFreeMatchPool;				// �ȴ�ƥ�������б�,���������֮����Ⱥ�˳��
};

#endif