#ifndef _MAHJONG_ROBOT_MANAGER_H_
#define _MAHJONG_ROBOT_MANAGER_H_

#include "FrameComponent.h"

class CharacterMahjongRobot;
class AccountTable;
class MahjongRobotManager : public FrameComponent
{
public:
	MahjongRobotManager(const std::string& name)
		:FrameComponent(name){}
	virtual ~MahjongRobotManager() { destroy(); }
	void destroy();
	virtual void init();
	CharacterMahjongRobot* createRobot();
protected:
	std::string generateRobotAccount();
	std::string generateRobotPassword();
	std::string generateRobotName();
	CharacterMahjongRobot* loginRobot(CHAR_GUID guid);
	CHAR_GUID registeRobot();
protected:
	txMap<CHAR_GUID, CharacterMahjongRobot*> mRobotList;
	txMap<CHAR_GUID, AccountTable*> mRobotAccountList;
};

#endif