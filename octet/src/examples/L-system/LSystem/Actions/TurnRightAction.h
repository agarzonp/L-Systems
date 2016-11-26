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

		octet::mat4t matrix;
		matrix.loadIdentity();
		matrix.rotateZ(angle);

		// transform the heading
		graphic.currentState().heading = matrix.rmul(octet::vec4(graphic.currentState().heading, 0)).normalize();
	}

private:

};

#endif
