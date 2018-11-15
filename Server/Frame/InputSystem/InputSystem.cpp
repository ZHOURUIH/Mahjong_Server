#include "InputSystem.h"

InputSystem::InputSystem(const string& name)
:
FrameComponent(name)
{
	mCurMouseWheelDelta = 0.0f;
	KEY_STATE state;
	state.mPushTime = 0.0f;
	state.mState = KPS_KEEP_UP;
	mMouseState.insert(MBD_LEFT, state);
	mMouseState.insert(MBD_RIGHT, state);
	mMouseState.insert(MBD_MIDDLE, state);
}

void InputSystem::update(float elapsedTime)
{
	auto iter = mKeyState.begin();
	auto iterEnd = mKeyState.end();
	for (; iter != iterEnd; ++iter)
	{
		iter->second.mPushTime += elapsedTime;
		if (iter->second.mState == KPS_CURRENT_UP)
		{
			iter->second.mState = KPS_KEEP_UP;
		}
		else if (iter->second.mState == KPS_CURRENT_DOWN)
		{
			iter->second.mState = KPS_KEEP_DOWN;
		}
	}

	auto iterMouse = mMouseState.begin();
	auto iterMouseEnd = mMouseState.end();
	for (; iterMouse != iterMouseEnd; ++iterMouse)
	{
		iterMouse->second.mPushTime += elapsedTime;
		if (iterMouse->second.mState == KPS_CURRENT_UP)
		{
			iterMouse->second.mState = KPS_KEEP_UP;
		}
		else if (iterMouse->second.mState == KPS_CURRENT_DOWN)
		{
			iterMouse->second.mState = KPS_KEEP_DOWN;
		}
	}

	// 清空当前按下的按键列表和字符列表
	mCurDownKeyList.clear();
	mCurCharList.clear();
	mCurMouseWheelDelta = 0.0f;
	mLastMousePosition = mCurMousePosition;
}

void InputSystem::keyDown(int key)
{
	auto iter = mKeyState.find(key);
	if (iter == mKeyState.end())
	{
		KEY_STATE keyState;
		keyState.mPushTime = 0.0f;
		keyState.mState = KPS_CURRENT_DOWN;
		mKeyState.insert(key, keyState);
		mCurDownKeyList.insert(key);
	}
	else
	{
		if (iter->second.mState == KPS_CURRENT_UP || iter->second.mState == KPS_KEEP_UP)
		{
			iter->second.mState = KPS_CURRENT_DOWN;
			iter->second.mPushTime = 0.0f;
			mCurDownKeyList.insert(key);
		}
		else if (iter->second.mState == KPS_CURRENT_DOWN || iter->second.mState == KPS_KEEP_DOWN)
		{
			iter->second.mState = KPS_KEEP_DOWN;
			iter->second.mPushTime = 0.0f;
		}
	}
}

void InputSystem::keyUp(int key)
{
	auto iter = mKeyState.find(key);
	if (iter == mKeyState.end())
	{
		KEY_STATE keyState;
		keyState.mPushTime = 0.0f;
		keyState.mState = KPS_CURRENT_UP;
		mKeyState.insert(key, keyState);
	}
	else
	{
		if (iter->second.mState == KPS_CURRENT_UP || iter->second.mState == KPS_KEEP_UP)
		{
			iter->second.mState = KPS_KEEP_UP;
			iter->second.mPushTime = 0.0f;
		}
		else if (iter->second.mState == KPS_CURRENT_DOWN || iter->second.mState == KPS_KEEP_DOWN)
		{
			iter->second.mState = KPS_CURRENT_UP;
			iter->second.mPushTime = 0.0f;
		}
	}
}

bool InputSystem::isKeyDown(int key)
{
	auto iter = mKeyState.find(key);
	if (iter != mKeyState.end())
	{
		return iter->second.mState == KPS_CURRENT_DOWN || iter->second.mState == KPS_KEEP_DOWN;
	}
	return false;
}

bool InputSystem::isKeyCurrentDown(int key)
{
	auto iter = mKeyState.find(key);
	if (iter != mKeyState.end())
	{
		return iter->second.mState == KPS_CURRENT_DOWN;
	}
	return false;
}

bool InputSystem::isKeyCurrentUp(int key)
{
	auto iter = mKeyState.find(key);
	if (iter != mKeyState.end())
	{
		return iter->second.mState == KPS_CURRENT_UP;
	}
	return false;
}

float InputSystem::getKeyTime(int key)
{
	auto iter = mKeyState.find(key);
	if (iter != mKeyState.end())
	{
		return iter->second.mPushTime;
	}
	return 0.0f;
}
