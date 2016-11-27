#ifndef L_SYSTEM_BRANCH_H
#define L_SYSTEM_BRANCH_H

#include <vector>
#include <cassert>

#include "LSystemMesh.h"
#include "LSystemTreeColour.h"

class LSystemBranch
{
	// unique identifier for this branch
	unsigned id_;

	// parent branch: to keep track where we can from
	LSystemBranch* parent_;

	// children branches: to keep track the different branches that the current branch creates
	std::vector<LSystemBranch*> children;

	// the key vertices that the current branch has
	std::vector<VecVertex> vertices;

	// meshes
	LSystemMesh branchMesh;
	LSystemMesh leafMesh;

public:

	LSystemBranch()
	{
		Clear();
	}

	void Init(unsigned id, LSystemBranch* parent)
	{
		id_ = id;
		parent_ = parent;
	}

	const LSystemBranch* parent() const { return parent_; }
	LSystemBranch*& parent() { return parent_; }

	const unsigned& id() const { return id_; }
	unsigned& id() { return id_; }

	void AddChild(LSystemBranch* child)
	{
		children.push_back(child);
	}

	LSystemBranch* GetChild(size_t index)
	{
		assert(index < children.size());
		if (index < children.size())
		{
			return children[index];
		}

		return nullptr;
	}

	size_t ChildCount() const
	{
		return children.size();
	}

	void AddVertex(VecVertex vertex)
	{
		vertices.push_back(vertex);
	}

	const VecVertex* GetVertex(size_t index) const
	{
		assert(index < vertices.size());
		if (index < vertices.size())
		{
			return &vertices[index];
		}

		return nullptr;
	}

	size_t VertexCount() const
	{
		return vertices.size();
	}

	void Clear()
	{
		id_ = 0;
		parent_ = nullptr;
		children.clear();
		vertices.clear();
		branchMesh.Clear();
		leafMesh.Clear();
	}

	void CreateMesh()
	{
		assert(vertices.size() > 1);
		if (vertices.size() > 1)
		{
			// create branch mesh
			CreateBranchMesh();

			// create leaf mesh
			CreateLeafMesh();
		}
	}

	void Draw(octet::color_shader& shader, const octet::mat4t& modelToProjection, int branchDepth) const
	{
		// draw a branch with a color
		octet::vec4 emissive_color = LSystemTreeColour::GetInstance()->GetBranchColour(branchDepth);

		shader.render(modelToProjection, emissive_color);

		branchMesh.Draw();

		// drawn leaf with other color
		emissive_color = LSystemTreeColour::GetInstance()->GetLeafColour(branchDepth);
		shader.render(modelToProjection, emissive_color);

		leafMesh.Draw();
	}

	private:

		void CreateBranchMesh()
		{
			branchMesh.Create(vertices);
		}

		void CreateLeafMesh()
		{
			float leafLength = 0.005f; // This should be in config

			VecVertex dir = (vertices.back() - vertices[vertices.size() - 2]).normalize();

			std::vector<VecVertex> leafVertices;
			leafVertices.push_back(vertices.back());
			leafVertices.push_back(vertices.back() + (dir * leafLength));

			leafMesh.Create(leafVertices);
		}
};

#endif // !L_SYSTEM_BRANCH_H

