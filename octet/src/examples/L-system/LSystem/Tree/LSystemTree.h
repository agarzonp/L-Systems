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

	void AddVertex(const VecVertex& vertex)
	{
		current->AddVertex(vertex);
	}

	void AddBranch(const VecVertex& vertex)
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

	void AddLeaf()
	{
		// pop the branch from the stack and make it the current branch
		assert(branchStack.size() > 0);
		if (branchStack.size())
		{
			current = branchStack.top();
			branchStack.pop();
		}

		// Each branch will create a leaf with the last two vertices
		// There is no real need to add a proper leaf at all
	}

	void CreateMesh()
	{
		CreateMesh(root);	
	}

	void Draw()
	{
		DrawBranch(root);
	}

	void Print()
	{
		printf("\n===== Tree =====\n");
		
		PrintBranch(root);
	}

private:

	void CreateMesh(LSystemBranch& branch)
	{
		if (branch.VertexCount() <= 1)
		{
			// do not create a mesh for branches that do not have at least 2 vertices
			return;
		}

		// create the mesh for current branch
		branch.CreateMesh();

		// create the mesh for all children branches
		for (size_t i = 0; i < branch.ChildCount(); i++)
		{
			LSystemBranch* childBranch = branch.GetChild(i);

			CreateMesh(*childBranch);
		}
	}

	void DrawBranch(LSystemBranch& branch)
	{
		// draw current branch
		branch.Draw();

		// draw all children
		for (size_t i = 0; i < branch.ChildCount(); i++)
		{
			LSystemBranch* childBranch = branch.GetChild(i);

			DrawBranch(*childBranch);
		}
	}

	void PrintBranch(LSystemBranch& branch)
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
			const VecVertex& vertex = *branch.GetVertex(i);
			printf("(%f, %f, %f)\n", vertex.x(), vertex.y(), vertex.z());
		}

		for (size_t i = 0; i < branch.ChildCount(); i++)
		{
			LSystemBranch* childBranch = branch.GetChild(i);

			PrintBranch(*childBranch);
		}
	}
};

#endif // !L_SYSTEM_TREE_H

