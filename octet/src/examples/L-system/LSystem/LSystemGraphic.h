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
	LSystemGraphicState currentState_;

	// stack to keep track of previous states
	std::stack<LSystemGraphicState> states;

	// The tree
	LSystemTree tree;

public:

	const LSystemGraphicState& currentState() const { return currentState_; }
	LSystemGraphicState& currentState() { return currentState_; }

	void AddCurrentVertex()
	{
		tree.AddVertex(currentState_.pos);
	}

	void PushState()
	{
		// push current state
		states.push(currentState_);

		// create a branch from currentState_
		tree.CreateBranch(currentState_.pos);
	}

	void PopState()
	{
		// create a leaf
		tree.CreateLeaf(); // TO-DO: pass some kind of randomness?

		// pop the state
		currentState_ = states.top();
		states.pop();
	}

	void Clear()
	{
		// Clear everything and reset
		currentState_.Clear();
		tree.Clear();

		AddCurrentVertex();
	}

	void Draw()
	{
		tree.Draw();
	}

	void Print()
	{
		tree.Print();
	}

private:

	void AddVertex(const octet::vec3& vertex)
	{
		tree.AddVertex(currentState_.pos);
	}
	
};

#endif // !L_SYSTEM_GRAPHIC_H

