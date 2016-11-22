#ifndef L_SYSTEM_ACTION_H
#define L_SYSTEM_ACTION_H

#include "../LSystemGraphic.h"

class LSystemAction
{
protected:

	LSystemAction()
	{
	}

public:

	virtual ~LSystemAction()
	{
	}

	virtual void Execute(LSystemGraphic& graphic) = 0;
};

#endif // !L_SYSTEM_ACTION_H
