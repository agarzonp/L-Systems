#ifndef L_SYSTEM_BRANCH_H
#define L_SYSTEM_BRANCH_H

#include <vector>
#include <cassert>

typedef octet::vec3 Vertex;

class LSystemBranch
{
	// parent branch: to keep track where we can from
	LSystemBranch* parent_;

	// children branches: to keep track the different branches that the current branch creates
	std::vector<LSystemBranch*> children;

	// the vertices that the current branch has
	std::vector<Vertex> vertices;

	// id
	unsigned id_;

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

	const LSystemBranch* GetChild(size_t index) const
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

	void AddVertex(Vertex vertex)
	{
		vertices.push_back(vertex);
	}

	const Vertex* GetVertex(size_t index) const
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
	}
};

#endif // !L_SYSTEM_BRANCH_H

