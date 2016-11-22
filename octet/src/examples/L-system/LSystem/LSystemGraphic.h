#ifndef L_SYSTEM_GRAPHIC_H
#define L_SYSTEM_GRAPHIC_H

#include <list>
#include <stack>

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
	LSystemGraphicState _currentState;

	// list of vertices to draw
	std::list<octet::vec3> vertices;

	// stack to keep track of previous states
	std::stack<LSystemGraphicState> states;
public:

	const LSystemGraphicState& currentState() const { return _currentState; }
	LSystemGraphicState& currentState() { return _currentState; }

	void AddVertex(const octet::vec3& vertex)
	{
		vertices.push_back(vertex);
	}

	void PushState(const LSystemGraphicState& state)
	{
		states.push(state);
	}

	void PopState()
	{
		_currentState = states.top();

		states.pop();
	}

	void Clear()
	{
		vertices.clear();
		_currentState.Clear();
	}

	void Draw()
	{
		// TO-DO: draw the list of vertices
		
		PrintVertices();
	}

private:

	void PrintVertices()
	{
		printf("\n\n\nGenerated Vertices\n==================\n\n");

		for (auto& vertex : vertices)
		{
			printf("(%f, %f, %f)\n", vertex.x(), vertex.y(), vertex.z());
		}
	}
};

#endif // !L_SYSTEM_GRAPHIC_H

