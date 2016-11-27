#ifndef L_SYSTEM_TREE_H
#define L_SYSTEM_TREE_H

#include <stack>
#include <string>
#include <cassert>

#include "../../Utils/Transform.h"
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


	// bounding square of the tree
	struct BoundingSquare
	{
		VecVertex topLeft;
		VecVertex bottomRight;

		void Clear()
		{
			topLeft = bottomRight = VecVertex(0.0f, 0.0f, 0.0f);
		}
	};

	BoundingSquare boundingSquare;

	// transformation applied to the tree
	Transform transform;

public:

	LSystemTree()
	{
		Clear();
	}

	void Clear()
	{
		transform.SetIdentity();

		boundingSquare.Clear();

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
		// add new vertex to current branch
		current->AddVertex(vertex);

		// update the bounding square
		UpdateBoundingSquare(vertex);
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

		// calculate the transform to make the mesh to be always on the screen
		CalculateTransform();
	}

	void Draw(octet::color_shader& shader)
	{
		// set the modelToProjection matrix to be our transform matrix
		octet::mat4t modelToProjection = transform.GetMatrix();

		// draw all branches
		int branchDepth = 0;
		DrawBranch(shader, modelToProjection, root, branchDepth);
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

	void DrawBranch(octet::color_shader& shader, const octet::mat4t& modelToProjection, LSystemBranch& branch, int branchDepth)
	{
		// draw current branch
		branch.Draw(shader, modelToProjection, branchDepth);

		// draw all branches with a different colour
		branchDepth++;

		// draw all children
		for (size_t i = 0; i < branch.ChildCount(); i++)
		{
			LSystemBranch* childBranch = branch.GetChild(i);

			DrawBranch(shader, modelToProjection, *childBranch, branchDepth);
		}
	}

	void UpdateBoundingSquare(const VecVertex& vertex)
	{
		boundingSquare.topLeft.x() = std::fminf(boundingSquare.topLeft.x(), vertex.x());
		boundingSquare.topLeft.y() = std::fmaxf(boundingSquare.topLeft.y(), vertex.y());
		boundingSquare.bottomRight.x() = std::fmaxf(boundingSquare.bottomRight.x(), vertex.x());
		boundingSquare.bottomRight.y() = std::fminf(boundingSquare.bottomRight.y(), vertex.y());
	}

	void CalculateTransform()
	{
		float offset = 0.6f;

		// set scale
		octet::vec3 scale(1.0f, 1.0f, 1.0f);

		float treeWidth = std::abs(boundingSquare.bottomRight.x() - boundingSquare.topLeft.x());
		float treeHeight = std::abs(boundingSquare.bottomRight.y() - boundingSquare.topLeft.y());

		if (treeHeight > 2.0f - offset)
		{
			float scaleFactor = (2.0f - offset) / treeHeight;
			
			scale.x() = scaleFactor;
			scale.y() = scaleFactor;
		}

		// set translation
		float dy = -offset;
		octet::vec3 translation(0.0f, dy, 0.0f);

		// set transfrom
		transform.SetIdentity();
		transform.Translation() = translation;
		transform.Scale() = scale;
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

