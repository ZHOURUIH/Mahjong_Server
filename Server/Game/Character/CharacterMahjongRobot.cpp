#include "CharacterMahjongRobot.h"
#include "CommandHeader.h"

void CharacterMahjongRobot::notifyStartGame()
{
	CommandCharacterGetStartDone* cmdStartDone = NEW_CMD_INFO(cmdStartDone);
	mCommandSystem->pushCommand(cmdStartDone, this);
}