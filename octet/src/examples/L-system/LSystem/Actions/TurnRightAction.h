#ifndef TURN_RIGHT_ACTION_H
#define TURN_RIGHT_ACTION_H

#include "LSystemAction.h"

class TurnRightAction : public LSystemAction
{
public:

	static const char* GetID() { return "+"; }

	TurnRightAction()
	{
	}

	~TurnRightAction()
	{

	}

	void Execute(LSystemGraphic& graphic) override
	{

	}

private:

};

#endif
