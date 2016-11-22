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

	void Execute(LSystemGraphic& graphic) override
	{
		
	}

private:

};

#endif

