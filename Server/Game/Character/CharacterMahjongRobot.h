#ifndef _CHARACTER_MAHJONG_ROBOT_H_
#define _CHARACTER_MAHJONG_ROBOT_H_

#include "CharacterPlayer.h"

class CharacterMahjongRobot : public CharacterPlayer
{
public:
	CharacterMahjongRobot(CHARACTER_TYPE type, const std::string& name)
		:
		CharacterPlayer(type, name)
	{}
	virtual ~CharacterMahjongRobot(){ destroy(); }
	void destroy(){}
	virtual void notifyStartGame();
};

#endif
