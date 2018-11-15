#ifndef _TX_COMMAND_H_
#define _TX_COMMAND_H_

#include "Utility.h"

#include "CommandDefine.h"
#include "ServerDefine.h"
#include "GameBase.h"

class CommandReceiver;
class txCommand : public GameBase
{
public:
	txCommand()
	{
		mReceiver = NULL;
		mShowDebugInfo = true;
		mDelayCommand = false;
		mValid = false;
		mExecuteState = ES_NOT_EXECUTE;
		mType = "";
		mFile = "";
		mLine = 0;
		mCmdID = 0;
		mAssignID = 0;
		mResult = NULL;
	}
	virtual ~txCommand(){}
	virtual void init();
	virtual void reset() = 0;
	// 命令执行
	virtual void execute() = 0;
	// 调试信息，由CommandSystem调用
	virtual string showDebugInfo()
	{
		COMMAND_DEBUG("");
	}
	bool getShowDebugInfo()						{ return mShowDebugInfo; }
	bool isDelayCommand()						{ return mDelayCommand; }
	CommandReceiver* getReceiver()				{ return mReceiver; }
	bool isValid()								{ return mValid; }
	const string& getType()						{ return mType; }
	EXECUTE_STATE getExecuteState()				{ return mExecuteState; }
	int getAssignID()							{ return mAssignID; }
	int getCmdID()								{ return mCmdID; }
	const string& getFile()						{ return mFile; }
	int getLine()								{ return mLine; }
	void setShowDebugInfo(bool show)			{ mShowDebugInfo = show; }
	void setDelayCommand(bool delay)			{ mDelayCommand = delay; }
	void setReceiver(CommandReceiver* Reciver){ mReceiver = Reciver; }
	void setValid(bool valid)					{ mValid = valid; }
	void setType(const string& type)			{ mType = type; }
	void setExecuteState(EXECUTE_STATE state)	{ mExecuteState = state; }
	void setAssignID(int id)					{ mAssignID = id; }
	void setID(int id)							{ mCmdID = id; }
	void setFile(const string& file)			{ mFile = file; }
	void setLine(int line)						{ mLine = line; }
	void setResult(bool* result)				{ mResult = result; }
	void addEndCommandCallback(CommandCallback cmdCallback, void* userData)
	{
		if (cmdCallback != NULL)
		{
			mEndCallback.push_back(make_pair(cmdCallback, userData));
		}
	}
	void addStartCommandCallback(CommandCallback cmdCallback, void* userData)
	{
		if (cmdCallback != NULL)
		{
			mStartCallback.push_back(make_pair(cmdCallback, userData));
		}
	}
	void runEndCallBack();
	void runStartCallBack();
protected:
	CommandReceiver* mReceiver;	// 命令接受者
	bool mShowDebugInfo;			// 是否显示调试信息
	bool mDelayCommand;				// 是否是延迟执行的命令
	bool mValid;					// 是否为有效命令
	EXECUTE_STATE mExecuteState;	// 命令执行状态
	string mType;				// 命令类型
	string mFile;				// 命令发出时所在的文件名
	int mLine;						// 命令发出时所在的行号
	int	mCmdID;						// 命令唯一ID,在命令被创建时设置
	int mAssignID;					// 被分配为延迟命令时的唯一ID,每次分配都会设置一个新的唯一执行ID
	bool* mResult;					// 命令的执行结果,只用于部分需要知道执行结果的命令使用
	txVector<pair<CommandCallback, void*>> mEndCallback;		// 命令执行完毕时的回调函数
	txVector<pair<CommandCallback, void*>> mStartCallback;	// 命令开始执行时的回调函数
};

#endif