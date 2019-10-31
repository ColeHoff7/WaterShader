#pragma once

/*
* Preprocessor statement for text substitution
*/
#define WINDOW_WIDTH 1024 // Default window width
#define WINDOW_HEIGHT 768 // Default window height

#include <iostream>
#include <vector>

using namespace std;

#include <GL/glew.h> 
#include <GL/freeglut.h> 

// Includes for model loading
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "freeImage/FreeImage.h"

#define GLM_SWIZZLE 

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

#define color glm::vec4

#define M_PI glm::pi<float>()


enum TextureMode { NO_TEXTURE = 0, DECAL, REPLACE_AMBIENT_DIFFUSE };

// Simple functions for printing vectors and matrices to the console
void print(const glm::vec2 & v0);

void print(const glm::vec3 & v0);

void print(const glm::vec4 & v0);

void print(const glm::mat2 & m);

void print(const glm::mat3 & m);

void print(const glm::mat4 & m);


glm::vec3 findUnitNormal(glm::vec3 pZero, glm::vec3 pOne, glm::vec3 pTwo);


color getRandomColor();


void displayOpenGlInfo(void);


void checkOpenGLErrors(const GLchar* methodName);

#include "BuildShader.h"
#include "SharedMaterialProperties.h"
#include "SharedProjectionAndViewing.h"
#include "SharedGeneralLighting.h"

struct pntVertexData
{
	glm::vec4 m_pos;
	glm::vec3 m_normal;
	glm::vec2 m_textCoord;

	pntVertexData() {}

	pntVertexData(glm::vec4 pos, glm::vec3 normal, glm::vec2 textCoord)
	{
		m_pos = pos;
		m_normal = normal;
		m_textCoord = textCoord;
	}

	pntVertexData(glm::vec3 pos, glm::vec3 normal, glm::vec2 textCoord)
	{
		pntVertexData(glm::vec4(pos, 1.0f), normal, textCoord);
	}
};
