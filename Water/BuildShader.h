#pragma once

#include "BasicIncludesAndDefines.h"

typedef struct {
	GLenum       type;
	const char*  filename;
	GLuint       shader;
} ShaderInfo;

GLuint BuildShaderProgram(ShaderInfo* shaders);

