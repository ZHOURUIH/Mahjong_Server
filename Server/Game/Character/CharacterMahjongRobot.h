#ifndef _CHARACTER_MAHJONG_ROBOT_H_
#define _CHARACTER_MAHJONG_ROBOT_H_

#include "CharacterPlayer.h"

class MahjongAction;
class CharacterMahjongRobot : public CharacterPlayer
{
public:
	CharacterMahjongRobot(CHARACTER_TYPE type, const std::string& name)
		:
		CharacterPlayer(type, name)
	{
		mAskAcitonCmdID = -1;
		mRequestDropCmdID = -1;
	}
	virtual ~CharacterMahjongRobot(){ destroy(); }
	void destroy() { clearCmd(); }
	virtual void update(float elapsedTime);
	virtual void notifyStartGame();
	virtual void notifyAskAction(txVector<MahjongAction*>& actionList);
	virtual void notifyAskDrop();
	virtual void notifyLeaveRoom();
protected:
	void clearCmd();
	static void onCmdStart(txCommand* cmd, void* user_data);
protected:
	int mAskAcitonCmdID;
	int mRequestDropCmdID;
};

#endif
