#include "Sphere.h"


// Get the Spherical texture mapping coordinates
glm::vec2 getSphericalTextCoords(float theta, glm::vec3 vertex)
{
	glm::vec3 d = glm::normalize(vertex);

	float s = theta / (2.0f * M_PI);

	float phi = acos(-d.z);
	float t = phi / M_PI;

	return glm::vec2(s, t);
}


Sphere::Sphere(float radius, int slices, int stacks)
:radius(radius), slices(slices), stacks(stacks), currentIndex(0)
{

}


Sphere::~Sphere()
{
}

void Sphere::initialize()
{
	stackStep = 2.0f * M_PI / (stacks*2.0f);
	sliceStep = 2.0f * M_PI / -slices;

	// Do the stacks
	z0 = 1.0;
	z1 = cos(stackStep);
	r0 = 0.0;
	r1 = sin(stackStep);

	initializeSphereTop();
	initializeSphereBody();
	initializeSphereBottom();

	// Generate vertex array object and bind it for the first time
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Create the buffer to hold interleaved and load data into it.
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Buffer for vertex data
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(pntVertexData), &v[0], GL_STATIC_DRAW); //Buffering vertex data

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const GLvoid*)sizeof(glm::vec4));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const GLvoid*)(sizeof(glm::vec4) + sizeof(glm::vec3)));
	glEnableVertexAttribArray(2);

	// Generate a buffer for the indices
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Save the number of indices for rendering
	numberOfElements = indices.size();

	// Now that vertex data and indices are buffered, these are no longer needed.
	v.clear();
	indices.clear();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Initialize the children
	VisibleObject::initialize();

} // end initialize


// Construct visual object display list.
void Sphere::initializeSphereTop()
{
	glm::vec3 normal = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec4 vTop = glm::vec4(0.0f, 0.0f, radius, 1.0f);
	float theta = 0.0f;

	for (int j = slices; j > 0; j--) {

		theta = j * sliceStep;
		glm::vec3 n0 = glm::vec3(cos(theta) * r1, sin(theta) * r1, z1);
		glm::vec4 v0 = glm::vec4(cos(theta) * r1 * radius, sin(theta) * r1 *radius, z1* radius, 1.0f);

		v.push_back(pntVertexData(v0, n0, getSphericalTextCoords(theta, n0)));
		indices.push_back(currentIndex++);

		theta = (j - 1) * sliceStep;
		n0 = glm::vec3(cos(theta) * r1, sin(theta) * r1, z1);
		v0 = glm::vec4(cos(theta) * r1 * radius, sin(theta) * r1 *radius, z1* radius, 1.0f);

		v.push_back(pntVertexData(v0, n0, getSphericalTextCoords(theta, n0)));
		indices.push_back(currentIndex++);

		theta = j * sliceStep - sliceStep / 2;
		v.push_back(pntVertexData(vTop, normal, getSphericalTextCoords(theta, normal)));
		indices.push_back(currentIndex++);
	}

} // end initializeSphereTop

void Sphere::initializeSphereBody()
{
	float theta = 0.0f;

	for (int i = 1; i < stacks - 1; i++)
	{
		z0 = z1;
		z1 = cos((i + 1)*stackStep);
		r0 = r1;
		r1 = sin((i + 1)*stackStep);

		for (int j = 0; j < slices; j++)
		{
			theta = j * sliceStep;
			glm::vec3 n01 = glm::normalize(glm::vec3(cos(theta) * r1, sin(theta) * r1, z1));
			glm::vec4 v0 = glm::vec4(cos(theta) * r1 * radius, sin(theta) * r1 * radius, z1 * radius, 1.0f);
			v.push_back(pntVertexData(v0, n01, getSphericalTextCoords(theta, n01))); // 0

			glm::vec3 n11 = glm::normalize(glm::vec3(cos(theta) * r0, sin(theta) * r0, z0));
			glm::vec4 v1 = glm::vec4(cos(theta) * r0 * radius, sin(theta) * r0 * radius, z0 * radius, 1.0f);
			v.push_back(pntVertexData(v1, n11, getSphericalTextCoords(theta, n11))); // 1

			theta = (j + 1) * sliceStep;
			glm::vec3 n23 = glm::normalize(glm::vec3(cos(theta) * r1, sin(theta) * r1, z1));
			glm::vec4 v2 = glm::vec4(cos(theta) * r1* radius, sin(theta) * r1 * radius, z1 * radius, 1.0f);
			v.push_back(pntVertexData(v2, n23, getSphericalTextCoords(theta, n23))); // 2

			glm::vec3 n33 = glm::normalize(glm::vec3(cos(theta) * r0, sin(theta) * r0, z0));
			glm::vec4 v3 = glm::vec4(cos(theta) * r0 *radius, sin(theta) * r0 *radius, z0 * radius, 1.0f);
			v.push_back(pntVertexData(v3, n33, getSphericalTextCoords(theta, n33))); // 3

			indices.push_back(currentIndex);
			indices.push_back(currentIndex + 3);
			indices.push_back(currentIndex + 2);

			indices.push_back(currentIndex);
			indices.push_back(currentIndex + 1);
			indices.push_back(currentIndex + 3);

			currentIndex += 4;
		}
	}

} // end initializeSphereBody

// Construct visual object display list.
void Sphere::initializeSphereBottom()
{
	float theta = 0.0f;

	z0 = z1;
	r0 = r1;

	glm::vec4 vBottom = glm::vec4(0.0f, 0.0f, -radius, 1.0f);
	glm::vec3 normal = glm::vec3(0.0f, 0.0f, -1.0f);

	for (int j = 0; j < slices; j++) {

		theta = j * sliceStep;
		glm::vec3 n0 = glm::vec3(cos(theta) * r0, sin(theta) * r0, z1);
		glm::vec4 v0 = glm::vec4(cos(theta) * r0 * radius, sin(theta) * r0 * radius, z0* radius, 1.0f);

		v.push_back(pntVertexData(v0, n0, getSphericalTextCoords(theta, n0)));
		indices.push_back(currentIndex++);

		theta = (j + 1) * sliceStep;
		n0 = glm::vec3(cos(theta) * r0, sin(theta) * r0, z1);
		v0 = glm::vec4(cos(theta) * r0 * radius, sin(theta) * r0 * radius, z0* radius, 1.0f);

		v.push_back(pntVertexData(v0, n0, getSphericalTextCoords(theta, n0)));
		indices.push_back(currentIndex++);

		theta = (j)* sliceStep + sliceStep / 2;
		v.push_back(pntVertexData(vBottom, normal, getSphericalTextCoords(theta, normal)));
		indices.push_back(currentIndex++);
	}

} // end initializeSphereBottom



