#ifndef _COMMAND_RECEIVER_H_
#define _COMMAND_RECEIVER_H_

#include "ServerDefine.h"
#include "GameBase.h"

class txCommand;
class CommandReceiver : public GameBase
{
public:
	CommandReceiver(const string& name);
	virtual ~CommandReceiver();
	virtual void receiveCommand(txCommand* cmd);
	const string& getName() { return mName; }
	// 谨慎使用设置名字
	void setName(const string& name) { mName = name; }
protected:
	string mName;
};

#endif