#pragma once

#include "BasicIncludesAndDefines.h"

#include "VisibleObject.h"
#include "Mesh.h"


class MyScene : public VisibleObject
{
public:
	MyScene();

	~MyScene();

	virtual void initialize();

	virtual bool update(float deltaTime);

	void resize(int windowWidth, int windowHeight);

	void toggleLights(int light);

	void setTextureMode(int mode);

	virtual void draw();

	void setProj(int i);

protected:

	GLuint perPixelShaderProgram;

	// Objects in the scene
	Mesh* mesh;

	int proj = 1;

};

