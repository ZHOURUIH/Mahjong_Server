#ifndef _FRAME_COMPONENT_H_
#define _FRAME_COMPONENT_H_

#include "txCommandReceiver.h"

class FrameComponent : public txCommandReceiver
{
public:
	FrameComponent(const std::string& name)
		:txCommandReceiver(name) {}
	virtual void init() {}
	virtual void update(float elapsedTime) {}
};

#endif