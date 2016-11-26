#ifndef PUSH_STATE_ACTION_H
#define PUSH_STATE_ACTION_H

#include "LSystemAction.h"

class PushStateAction : public LSystemAction
{
public:

	static const char* GetID() { return "["; }

	PushStateAction()
	{
	}

	~PushStateAction()
	{

	}

	void Execute(LSystemGraphic& graphic, const LSystemConfig& config) override
	{
		graphic.PushState();
	}

private:

};

#endif
