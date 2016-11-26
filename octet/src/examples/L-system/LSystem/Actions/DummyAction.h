#ifndef DUMMY_ACTION_H
#define DUMMY_ACTION_H

#include "LSystemAction.h"

class DummyAction : public LSystemAction
{
public:

	static const char* GetID() { return " "; }

	DummyAction()
	{
	}

	~DummyAction()
	{

	}

	void Execute(LSystemGraphic& graphic, const LSystemConfig& config) override
	{

	}

private:

};

#endif
