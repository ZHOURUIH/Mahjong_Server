#ifndef _TX_COMMAND_H_
#define _TX_COMMAND_H_

#include "txUtility.h"

#include "CommandDefine.h"
#include "ServerDefine.h"
#include "CharacterDefine.h"
#include "ServerBase.h"

class txCommandReceiver;
class txCommand : public ServerBase
{
public:
	txCommand(){}
	virtual ~txCommand(){}
	virtual void init();
	virtual void reset() = 0;
	// 鍛戒护鎵ц
	virtual void execute() = 0;
	// 璋冭瘯淇℃伅锛岀敱CommandSystem璋冪敤
	virtual std::string showDebugInfo()
	{
		COMMAND_DEBUG(DEBUG_EMPTY);
	}
	const bool& getShowDebugInfo()						{ return mShowDebugInfo; }
	const bool& isDelayCommand()						{ return mDelayCommand; }
	txCommandReceiver* getReceiver()					{ return mReceiver; }
	const bool& isValid()								{ return mValid; }
	const std::string& getType()						{ return mType; }
	const EXECUTE_STATE& getExecuteState()				{ return mExecuteState; }
	const int& getAssignID()							{ return mAssignID; }
	const int& getCmdID()								{ return mCmdID; }
	const std::string& getFile()						{ return mFile; }
	const int& getLine()								{ return mLine; }
	void setShowDebugInfo(const bool& show)				{ mShowDebugInfo = show; }
	void setDelayCommand(const bool& delay)				{ mDelayCommand = delay; }
	void setReceiver(txCommandReceiver* Reciver)		{ mReceiver = Reciver; }
	void setValid(const bool& valid)					{ mValid = valid; }
	void setType(const std::string& type)				{ mType = type; }
	void setExecuteState(const EXECUTE_STATE& state)	{ mExecuteState = state; }
	void setAssignID(const int& id)						{ mAssignID = id; }
	void setID(const int& id)							{ mCmdID = id; }
	void setFile(const std::string& file)				{ mFile = file; }
	void setLine(const int& line)						{ mLine = line; }
	void setResult(bool* result)						{ mResult = result; }
	void addEndCommandCallback(CommandCallback cmdCallback, void* userData)
	{
		if (cmdCallback != NULL)
		{
			mEndCallback.push_back(std::make_pair(cmdCallback, userData));
		}
	}
	void addStartCommandCallback(CommandCallback cmdCallback, void* userData)
	{
		if (cmdCallback != NULL)
		{
			mStartCallback.push_back(std::make_pair(cmdCallback, userData));
		}
	}
	void runEndCallBack();
	void runStartCallBack();
protected:
	txCommandReceiver* mReceiver;	// 鍛戒护鎺ュ彈鑰?
	bool mShowDebugInfo;			// 鏄惁鏄剧ず璋冭瘯淇℃伅
	bool mDelayCommand;				// 鏄惁鏄欢杩熸墽琛岀殑鍛戒护
	bool mValid;					// 鏄惁涓烘湁鏁堝懡浠?
	EXECUTE_STATE mExecuteState;	// 鍛戒护鎵ц鐘舵€?
	std::string mType;				// 鍛戒护绫诲瀷
	std::string mFile;				// 鍛戒护鍙戝嚭鏃舵墍鍦ㄧ殑鏂囦欢鍚?
	int mLine;						// 鍛戒护鍙戝嚭鏃舵墍鍦ㄧ殑琛屽彿
	int	mCmdID;						// 鍛戒护鍞竴ID,鍦ㄥ懡浠よ鍒涘缓鏃惰缃?
	int mAssignID;					// 琚垎閰嶄负寤惰繜鍛戒护鏃剁殑鍞竴ID,姣忔鍒嗛厤閮戒細璁剧疆涓€涓柊鐨勫敮涓€鎵цID
	bool* mResult;					// 鍛戒护鐨勬墽琛岀粨鏋?鍙敤浜庨儴鍒嗛渶瑕佺煡閬撴墽琛岀粨鏋滅殑鍛戒护浣跨敤
	txVector<std::pair<CommandCallback, void*>> mEndCallback;		// 鍛戒护鎵ц瀹屾瘯鏃剁殑鍥炶皟鍑芥暟
	txVector<std::pair<CommandCallback, void*>> mStartCallback;	// 鍛戒护寮€濮嬫墽琛屾椂鐨勫洖璋冨嚱鏁?
};

#endif