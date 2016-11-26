#ifndef L_SYSTEM_MESH
#define L_SYSTEM_MESH

typedef octet::vec3 VecVertex;

struct LSystemMeshVertex
{
	float pos[3];
};

class LSystemMesh
{
	enum
	{
		VERTEX_BUFFER_0,

		NUM_VERTEX_BUFFERS
	};

	GLuint vertexArrayObject;
	GLuint vertexBuffers[NUM_VERTEX_BUFFERS];

	LSystemMeshVertex* vertex_data;
	size_t numVertices;

public:

	LSystemMesh()
		: vertex_data(nullptr)
		, numVertices(0)
	{
	}
		

	void Clear()
	{
		numVertices = 0;

		if (vertex_data)
		{
			delete[] vertex_data;
			vertex_data = nullptr;

			glDeleteBuffers(NUM_VERTEX_BUFFERS, vertexBuffers);
			glDeleteVertexArrays(1, &vertexArrayObject);
		}	
	}

	void Create(const std::vector<VecVertex>& vecVertex)
	{
		/*
		static float d = 0.0f;
		numVertices = 2;
		vertex_data = new LSystemMeshVertex[numVertices];
		vertex_data[0].pos[0] = -0.5f;
		vertex_data[0].pos[1] = 0.0f + d;
		vertex_data[0].pos[2] = 0.0f;
		vertex_data[1].pos[0] = 0.5f;
		vertex_data[1].pos[1] = 0.0f + d;
		vertex_data[1].pos[2] = 0.0f;

		d += 0.25f;
		*/

		// create mesh vertices
		numVertices = vecVertex.size();
		vertex_data = new LSystemMeshVertex[numVertices];
		for (int i = 0; i < numVertices; i++)
		{
			vertex_data[i].pos[0] = vecVertex[i].x();
			vertex_data[i].pos[1] = vecVertex[i].y();
			vertex_data[i].pos[2] = vecVertex[i].z();
		}

		// create and use the VAO
		glGenVertexArrays(1, &vertexArrayObject);
		glBindVertexArray(vertexArrayObject);
	
		// create the buffer and set buffer data
		glGenBuffers(NUM_VERTEX_BUFFERS, vertexBuffers);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[VERTEX_BUFFER_0]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(LSystemMeshVertex), vertex_data, GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	void Draw() const
	{
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[VERTEX_BUFFER_0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL); // LSystemMeshVertex::pos
		glDrawArrays(GL_LINE_STRIP, 0, (GLsizei) numVertices);
		
		glDisableVertexAttribArray(0);
	}
};

#endif
