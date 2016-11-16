#ifndef L_SYSTEM_ACTION_H
#define L_SYSTEM_ACTION_H

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

	virtual void Execute() = 0;
};

#endif // !L_SYSTEM_ACTION_H
