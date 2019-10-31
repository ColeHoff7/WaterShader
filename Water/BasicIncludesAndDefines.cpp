#include "BasicIncludesAndDefines.h"

color getRandomColor()
{
	float red = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float green = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float blue = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	return color(red, green, blue, 1.0f);

} // end getRandomColor

void print(const glm::vec2 & v0)
{
	cout << "[ " << v0.x << " " << v0.y << " ]" << endl;
}

void print(const glm::vec3 & v0)
{
	cout << "[ " << v0.x << " " << v0.y << " " << v0.z << " " << " ]" << endl;
}

void print(const glm::vec4 & v0)
{
	cout << "[ " << v0.x << " " << v0.y << " " << v0.z << " " << v0.w << " ]" << endl;
}

void print(const glm::mat2 & m)
{
	std::cout << "\n";

	for (int row = 0; row < 2; row++) {
		std::cout << "|\t";
		for (int col = 0; col < 2; col++) {
			std::cout << m[col][row] << "\t";
		}
		std::cout << "|\n";
	}
	std::cout << "\n";

}

void print(const glm::mat3 & m)
{
	std::cout << "\n";
	for (int row = 0; row < 3; row++) {
			{
				std::cout << "|\t";
				for (int col = 0; col < 3; col++) {
					std::cout << m[col][row] << "\t";
				}
			}
		std::cout << "|\n";
	}
	std::cout << "\n";
}

void print(const glm::mat4 & m)
{
	std::cout << "\n";

	for (int row = 0; row < 3; row++) {
		std::cout << "|\t";
		for (int col = 0; col < 4; col++) {
			std::cout << m[col][row] << "\t";
		}
		std::cout << "|\n";
	}
	std::cout << "\n";

}

// Give three vertices on the face of a polygon in counter clockwise 
// order calculates a normal vector for the polygon.
glm::vec3 findUnitNormal(glm::vec3 pZero, glm::vec3 pOne, glm::vec3 pTwo)
{
	return glm::normalize(glm::cross(pOne - pZero, pTwo - pOne));

} // end findUnitNormal

/**
Function: Displays OpengGL and GLSL version information.
*/
void displayOpenGlInfo(void)
{
	// Display the company responsible for this GL implementation
	fprintf(stdout, "OpenGL INFO:\n\tOpenGL Implementor: %s\n",
		glGetString(GL_VENDOR));
	// Display the renderer/graphics card
	fprintf(stdout, "\tRenderer: %s\n", glGetString(GL_RENDERER));
	// Display the OpenGL version that is implemented
	fprintf(stdout, "\tOpenGL Version: %s\n", glGetString(GL_VERSION));
	// Display the GLSL version
	fprintf(stdout, "\tGLSL Version: %s\n\n",
		glGetString(GL_SHADING_LANGUAGE_VERSION));

} // end displayOpenGlInfo


void checkOpenGLErrors(const GLchar* methodName)
{
	GLenum errorCode; // Current error code.
	const GLubyte *errorString; // Desriptive string for the error. 

	if ((errorCode = glGetError()) != GL_NO_ERROR) {

		errorString = gluErrorString(errorCode);
		cout << errorString << " in " << methodName << endl;
	}

} // end checkOpenGLErrors