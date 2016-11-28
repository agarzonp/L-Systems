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

	void Execute(LSystemGraphic& graphic, const LSystemConfig& config) override
	{
		// build rotation matrix
		float angle = config.leftAngle;

		octet::mat4t matrix;
		matrix.loadIdentity();
		matrix.rotateZ(-angle);

		// rotate the graphic
		graphic.Rotate(matrix);
	}

private:

};

#endif
