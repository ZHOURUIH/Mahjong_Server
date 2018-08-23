#ifndef _MAHJONG_ROBOT_MANAGER_H_
#define _MAHJONG_ROBOT_MANAGER_H_

#include "FrameComponent.h"

class CharacterMahjongRobot;
class MahjongRobotManager : public FrameComponent
{
public:
	MahjongRobotManager(const std::string& name)
		:FrameComponent(name){}
	virtual ~MahjongRobotManager() { destroy(); }
	void destroy();
	virtual void init(){}
	CharacterMahjongRobot* createRobot();
protected:
	std::string generateRobotAccount();
	std::string generateRobotPassword();
	std::string generateRobotName();
protected:
	txMap<std::string, CharacterMahjongRobot*> mRobotList;
};

#endif