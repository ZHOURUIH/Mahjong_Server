#ifndef _CUSTOM_THREAD_H_
#define _CUSTOM_THREAD_H_

#include "ServerDefine.h"

class TimeLock;
class CustomThread
{
public:
	CustomThread(const std::string& name);
	~CustomThread();
	void destroy();
	void setBackground(const bool& background){mIsBackground = background;}
	void start(CustomThreadCallback callback, void* args, const int& frameTimeMS = 15);
	void pause(const bool& isPause){mPause = isPause;}
	void stop();
protected:
	THREAD_CALLBACK_DECLEAR(run)
	{
		((CustomThread*)args)->updateThread();
		return NULL;
	}
	// ��дһ������,�����ھ�̬������ֻ��ͨ���������ʳ�Ա����
	void updateThread();
protected:
	volatile std::atomic<bool> mRunning;
	volatile std::atomic<bool> mFinish;
	volatile std::atomic<bool> mPause;
	volatile std::atomic<TX_THREAD> mThread;
	CustomThreadCallback mCallback;
	TimeLock* mTimeLock;
	std::string mName;
	bool mIsBackground;		// �Ƿ�Ϊ��̨�߳�,����Ǻ�̨�߳�,����Ӧ�ó���ر�ʱ,���̻߳��Զ�ǿ�ƹر�
	void* mArgs;
};

#endif