#ifndef _FRAME_COMPONENT_H_
#define _FRAME_COMPONENT_H_

#include "CommandReceiver.h"

class FrameComponent : public CommandReceiver
{
public:
	FrameComponent(const string& name)
		:CommandReceiver(name) {}
	virtual void init() {}
	virtual void update(float elapsedTime) {}
};

#endif