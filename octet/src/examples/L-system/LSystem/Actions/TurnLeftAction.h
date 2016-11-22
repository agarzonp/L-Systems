#ifndef TURN_LEFT_ACTION_H
#define TURN_LEFT_ACTION_H

#include "LSystemAction.h"

class TurnLeftAction : public LSystemAction
{
public:

	static const char* GetID() { return "-"; }

	TurnLeftAction()
	{
	}

	~TurnLeftAction()
	{

	}

	void Execute(LSystemGraphic& graphic) override
	{

	}

private:

};

#endif
