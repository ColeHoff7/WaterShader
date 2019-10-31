#include "Mesh.h"

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

void Mesh::initialize()
{

	vector<pntVertexData> v;
	int dim = 300;
	float unit = 1.f;
	float x, z;
	z = -dim / 2.f;
	glm::vec3 n = glm::vec3(0, 1, 0);
	for (int i = 0; i < dim / unit; i++) {
		x = -dim / 2.f;
		for (int j = 0; j < dim / unit; j++) {
			glm::vec4 v0(x, 0.f, z, 1.0f);
			glm::vec4 v1(x, 0.f, z+unit, 1.0f);
			glm::vec4 v2(x + unit, 0.f, z + unit, 1.0f);
			glm::vec4 v3(x + unit, 0.f, z, 1.0f);
			
			v.push_back(pntVertexData(v0, n, glm::vec2(x,z)));
			v.push_back(pntVertexData(v1, n, glm::vec2(x, z+unit)));
			v.push_back(pntVertexData(v3, n, glm::vec2(x+unit, z)));

			v.push_back(pntVertexData(v1, n, glm::vec2(x, z)));
			v.push_back(pntVertexData(v2, n, glm::vec2(x+unit, z+unit)));
			v.push_back(pntVertexData(v3, n, glm::vec2(x+unit, z)));
			
			x += unit;
		}
		z += unit;
	}


	// Generate vertex array object and bind it for the first time
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(pntVertexData), &v[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const GLvoid*)sizeof(glm::vec4));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec4)));
	glEnableVertexAttribArray(2);

	numberOfElements = v.size();
	v.clear();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Initialize the children
	VisibleObject::initialize();

}
