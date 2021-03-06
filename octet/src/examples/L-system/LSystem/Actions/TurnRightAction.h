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

	void Execute(LSystemGraphic& graphic, const LSystemConfig& config) override
	{
		// build rotation matrix
		float angle = config.rightAngle;

		octet::mat4t matrix;
		matrix.loadIdentity();
		matrix.rotateZ(angle);

		// rotate the graphic
		graphic.Rotate(matrix);
	}

private:

};

#endif
