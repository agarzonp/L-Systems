#ifndef POP_STATE_ACTION_H
#define POP_STATE_ACTION_H

#include "LSystemAction.h"

class PopStateAction : public LSystemAction
{
public:

	static const char* GetID() { return "]"; }

	PopStateAction()
	{
	}

	~PopStateAction()
	{

	}

	void Execute(LSystemGraphic& graphic) override
	{

	}

private:

};

#endif
