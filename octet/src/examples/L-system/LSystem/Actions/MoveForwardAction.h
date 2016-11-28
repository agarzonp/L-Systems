#ifndef MOVE_FORWARD_ACTION_H
#define MOVE_FORWARD_ACTION_H

#include "LSystemAction.h"

class MoveForwardAction : public LSystemAction
{
public:

	static const char* GetID() { return "F"; }

	MoveForwardAction()
	{
	}

	~MoveForwardAction()
	{

	}

	void Execute(LSystemGraphic& graphic, const LSystemConfig& config) override
	{
		float distance = 0.01f; // This should be in config
		graphic.MoveForward(distance);
	}

private:

};

#endif

