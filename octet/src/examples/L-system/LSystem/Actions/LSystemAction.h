#ifndef L_SYSTEM_ACTION_H
#define L_SYSTEM_ACTION_H

#include "../LSystemGraphic.h"
#include "../LSystemConfig.h"

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

	virtual void Execute(LSystemGraphic& graphic, const LSystemConfig& config) = 0;
};

#endif // !L_SYSTEM_ACTION_H
