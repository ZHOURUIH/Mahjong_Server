#ifndef _INPUT_SYSTEM_H_
#define _INPUT_SYSTEM_H_

#include "FrameComponent.h"

// 按键状态
enum KEY_PUSH_STATE
{
	KPS_KEEP_UP, // 持续放开
	KPS_KEEP_DOWN, // 持续按下
	KPS_CURRENT_UP, // 当前放开
	KPS_CURRENT_DOWN, // 当前按下
};

// 鼠标按钮
enum MOUSE_BUTTON_DEFINE
{
	MBD_NONE,
	MBD_LEFT,
	MBD_RIGHT,
	MBD_MIDDLE,
};

struct KEY_STATE
{
	KEY_PUSH_STATE mState; // 按键的状态
	float mPushTime; // 放开或者按下的持续时间
};

class InputSystem : public FrameComponent
{
public:
	InputSystem(const string& name);
	virtual ~InputSystem() {}
	virtual void init() {}
	virtual void update(float elapsedTime);
	void keyDown(int key);
	void keyUp(int key);
	void onChar(char key)
	{
		mCurCharList.insert(key); 
	}

	bool isKeyDown(int key);
	bool isKeyCurrentDown(int key);
	bool isKeyCurrentUp(int key);
	float getKeyTime(int key);
	void mouseLeftDown(Vector2& position)
	{
		mMouseState[MBD_LEFT].mPushTime = 0.0f;
		mMouseState[MBD_LEFT].mState = KPS_CURRENT_DOWN;
		setCurMousePosition(position);
	}

	void mouseRightDown(Vector2& position)
	{
		mMouseState[MBD_RIGHT].mPushTime = 0.0f;
		mMouseState[MBD_RIGHT].mState = KPS_CURRENT_DOWN;
		setCurMousePosition(position);
	}

	void mouseMiddleDown(Vector2& position)
	{
		mMouseState[MBD_MIDDLE].mPushTime = 0.0f;
		mMouseState[MBD_MIDDLE].mState = KPS_CURRENT_DOWN;
		setCurMousePosition(position);
	}

	void mouseLeftUp(Vector2& position)
	{
		mMouseState[MBD_LEFT].mPushTime = 0.0f;
		mMouseState[MBD_LEFT].mState = KPS_CURRENT_UP;
		setCurMousePosition(position);
	}

	void mouseRightUp(Vector2& position)
	{
		mMouseState[MBD_RIGHT].mPushTime = 0.0f;
		mMouseState[MBD_RIGHT].mState = KPS_CURRENT_UP;
		setCurMousePosition(position);
	}

	void mouseMiddleUp(Vector2& position)
	{
		mMouseState[MBD_MIDDLE].mPushTime = 0.0f;
		mMouseState[MBD_MIDDLE].mState = KPS_CURRENT_UP;
		setCurMousePosition(position);
	}
	void mouseMove(Vector2& position)
	{
		setCurMousePosition(position); 
	}
	void mouseWheel(float delta)
	{
		mCurMouseWheelDelta = delta; 
	}
	void setCurMousePosition(Vector2& curPos)
	{
		mLastMousePosition = mCurMousePosition;
		mCurMousePosition = curPos;
	}
	KEY_STATE getMouseButtonState(MOUSE_BUTTON_DEFINE mouse){return mMouseState.tryGet(mouse, KEY_STATE());}
	txSet<int>& getCurDownKeyList() { return mCurDownKeyList; }
	txSet<int>& getCurCharList() { return mCurCharList; }
	Vector2& getMousePosition() { return mCurMousePosition; }
	Vector2& getLastMousePosition() { return mLastMousePosition; }
	float getMouseWheelDelta() { return mCurMouseWheelDelta; }
protected:
	txMap<int, KEY_STATE> mKeyState;
	txMap<MOUSE_BUTTON_DEFINE, KEY_STATE> mMouseState;
	Vector2 mLastMousePosition;
	Vector2 mCurMousePosition;
	float mCurMouseWheelDelta;
	txSet<int> mCurCharList;	// 当前按下的字符列表
	txSet<int> mCurDownKeyList;	// 所有处于当前按下状态的键
};

#endif
