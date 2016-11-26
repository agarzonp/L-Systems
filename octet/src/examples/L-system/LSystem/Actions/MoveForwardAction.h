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
		float distance = 0.01f;//10.0f; // This should be in config
		graphic.currentState().pos += graphic.currentState().heading * distance;

		graphic.AddCurrentVertex();
	}

private:

};

#endif

