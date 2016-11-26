#ifndef L_SYSTEM_TREE_H
#define L_SYSTEM_TREE_H

#include <stack>
#include <string>
#include <cassert>

#include "LSystemBranch.h"

class LSystemTree
{
	// The root of the tree
	LSystemBranch root;

	// current branch where we are creating the vertices
	LSystemBranch* current;

	// the stack to record the branches that we are creating
	std::stack<LSystemBranch*> branchStack;

	// branch count
	int branchCount;

public:

	LSystemTree()
	{
		Clear();
	}

	void Clear()
	{
		branchCount = 0;

		// clear the root
		root.Clear();

		// Clear the stack, taking into account that the one at the bottom is actually the root!
		for (size_t i = branchStack.size(); i > 1; i--)
		{
			LSystemBranch* branch = branchStack.top();
			branch->Clear();

			delete branch; // TO-DO: back to the pool!
			
			branchStack.pop();
		}

		// Make the current branch to be the root and initialise
		current = &root;
		current->Init(branchCount++, nullptr);
	}

	void AddVertex(const Vertex& vertex)
	{
		current->AddVertex(vertex);
	}

	void CreateBranch(const Vertex& vertex)
	{
		// Create a new branch and set the relationship between the current branch and the new one
		LSystemBranch* newBranch = new LSystemBranch(); // TO-DO: Use a pool!
		newBranch->Init(branchCount++, current);
		current->AddChild(newBranch);

		// add the first vertex to the new branch
		newBranch->AddVertex(vertex);

		// push the current branch into the stack
		branchStack.push(current);

		// set the new branch to be the current branch
		current = newBranch;
	}

	void CreateLeaf()
	{
		// pop the branch from the stack and make it the current branch
		assert(branchStack.size() > 0);
		if (branchStack.size())
		{
			current = branchStack.top();
			branchStack.pop();
		}

		// TO-DO: Create a leaf
	}

	void Draw()
	{
		// TO-DO: Draw the tree, create a mesh for each branch/leaf?
	}

	void Print()
	{
		printf("\n===== Tree =====\n");
		
		PrintBranch(root);
	}

private:

	void PrintBranch(const LSystemBranch& branch)
	{
		printf("\nBranch %d", branch.id());
		printf("\n==========");

		printf("\nParent branch: %d", branch.parent() ? branch.parent()->id() : -1);

		std::string childrenIds;
		for (size_t i = 0; i < branch.ChildCount(); i++)
		{
			const LSystemBranch* childBranch = branch.GetChild(i);

			childrenIds += " " + std::to_string(childBranch->id());
		}
		printf("\nChildren branches: %s\n", childrenIds.empty() ? "-1" : childrenIds.c_str());

		for (size_t i = 0; i < branch.VertexCount(); i++)
		{
			const Vertex& vertex = *branch.GetVertex(i);
			printf("(%f, %f, %f)\n", vertex.x(), vertex.y(), vertex.z());
		}

		for (size_t i = 0; i < branch.ChildCount(); i++)
		{
			const LSystemBranch* childBranch = branch.GetChild(i);

			PrintBranch(*childBranch);
		}
	}
};

#endif // !L_SYSTEM_TREE_H

