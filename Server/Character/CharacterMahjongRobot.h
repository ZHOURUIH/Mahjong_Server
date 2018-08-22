#ifndef _CHARACTER_MAHJONG_ROBOT_H_
#define _CHARACTER_MAHJONG_ROBOT_H_

#include "Character.h"

class CharacterMahjongRobot : public Character
{
public:
	CharacterMahjongRobot(CHARACTER_TYPE type, const std::string& name)
		:
		Character(type, name)
	{}
	virtual ~CharacterMahjongRobot(){ destroy(); }
	void destroy(){}
};

#endif
