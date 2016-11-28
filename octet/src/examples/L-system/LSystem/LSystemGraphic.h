#ifndef L_SYSTEM_GRAPHIC_H
#define L_SYSTEM_GRAPHIC_H

#include <list>
#include <stack>

#include "Tree/LSystemTree.h"

struct LSystemGraphicState
{
	// position
	octet::vec3 pos;

	// heading
	octet::vec3 heading;

	// clear function
	void Clear()
	{
		pos = octet::vec3(0, 0, 0);
		heading = octet::vec3(0, 1, 0);
	}
};

class LSystemGraphic
{
	// current state
	LSystemGraphicState currentState;

	// stack to keep track of previous states
	std::stack<LSystemGraphicState> states;

	// The tree
	LSystemTree tree;

public:

	void MoveForward(float distance)
	{
		currentState.pos += currentState.heading * distance;

		// add current vertex
		AddCurrentVertex();
	}

	void Rotate(octet::mat4t& rotation)
	{
		// transform the heading by the rotation matrix
		currentState.heading = rotation.rmul(octet::vec4(currentState.heading, 0)).normalize();
	}

	void PushState()
	{
		// push current state
		states.push(currentState);

		// add a branch from currentState_.pos
		tree.AddBranch(currentState.pos);
	}

	void PopState()
	{
		// add a leaf
		tree.AddLeaf();

		// pop the state
		currentState = states.top();
		states.pop();
	}

	void Clear()
	{
		// Clear everything and reset
		currentState.Clear();
		tree.Clear();

		AddCurrentVertex();
	}

	void Create()
	{
		tree.CreateMesh();
	}

	void Draw(octet::color_shader& shader)
	{
		tree.Draw(shader);
	}

	void Print()
	{
		tree.Print();
	}

private:

	void AddCurrentVertex()
	{
		tree.AddVertex(currentState.pos);
	}	
};

#endif // !L_SYSTEM_GRAPHIC_H

