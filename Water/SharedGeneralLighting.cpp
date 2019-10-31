#include "SharedGeneralLighting.h"

#include <sstream> 

vector<GeneralLight> SharedGeneralLighting::lights;

GLuint SharedGeneralLighting::generalLightBlockBuffer; // Identifier for the buffer. There would only be one buffer to feed several uniform blocks.
GLint SharedGeneralLighting::generalLightBlockSize = 0; // Size in bytes of both the buffer and the uniform blocks in all the the shaders. 
GLuint SharedGeneralLighting::generalLightBlockIndex; // Identifier for the uniform block.

bool SharedGeneralLighting::blockSizeAndOffetsSet = false; // Indicates whether or not the buffer for the blocks has been setup

const string SharedGeneralLighting::generalLightBlockName = "LightBlock";

bool checkFound(const GLchar* locationName, GLuint indice)
{
	if (indice == GL_INVALID_INDEX) {
		cout << locationName << " not found in shader." << endl;

		return false;
	}
	else {

		//cout << locationName << " index is " <<  indice << endl; 
		return true;
	}

} // end checkFound

void SharedGeneralLighting::setUniformBlockForShader(GLuint shaderProgram)
{
	determineBlockSizeSetBindingPoint(shaderProgram);

	if (blockSizeAndOffetsSet == false) {

		// Set up the buffers and bind to binding points
		allocateBuffers(shaderProgram);

		// Find the byte offsets of the uniform block variables
		findOffsets(shaderProgram);
	}

	checkOpenGLErrors("setUniformBlockForShader");

} // end setUniformBlockForShader

void SharedGeneralLighting::determineBlockSizeSetBindingPoint(GLuint shaderProgram)
{
	generalLightBlockIndex = glGetUniformBlockIndex(shaderProgram, generalLightBlockName.c_str());

	cout << generalLightBlockName.c_str() << " index is " << generalLightBlockIndex << endl;

	if (checkFound(generalLightBlockName.c_str(), generalLightBlockIndex)) {

		// Determine the size in bytes of the uniform block
		glGetActiveUniformBlockiv(shaderProgram, generalLightBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &generalLightBlockSize);
		cout << generalLightBlockName.c_str() << " size is " << generalLightBlockSize << endl;

		// Assign the block to a binding point
		glUniformBlockBinding(shaderProgram, generalLightBlockIndex, generalLightBlockBindingPoint);
	}

	checkOpenGLErrors("findBlockSizeSetBindingPoint");

} // end determineBlockSizeSetBindingPoint

void SharedGeneralLighting::allocateBuffers(GLuint shaderProgram)
{
	if (generalLightBlockSize > 0) {

		// Get an identifier for a buffer
		glGenBuffers(1, &generalLightBlockBuffer);

		glBindBuffer(GL_UNIFORM_BUFFER, generalLightBlockBuffer);

		// Allocate the buffer
		glBufferData(GL_UNIFORM_BUFFER, generalLightBlockSize, NULL, GL_DYNAMIC_DRAW);

		// Assign the buffer to a binding point
		glBindBufferBase(GL_UNIFORM_BUFFER, generalLightBlockBindingPoint, generalLightBlockBuffer);

		blockSizeAndOffetsSet = true;
	}

	checkOpenGLErrors("findOffets");

} // end allocateBuffers

void SharedGeneralLighting::findOffsets(GLuint shaderProgram)
{
	for (int i = 0; i < MAX_LIGHTS; i++) {

		GeneralLight light = initilizeLight(shaderProgram, i);

		lights.push_back(light);
	}

	for (int i = 0; i < MAX_LIGHTS; i++) {

		initilizeAttributes(i);

	}

}

GeneralLight SharedGeneralLighting::initilizeLight(GLuint shaderProgram, GLint lightNumber)
{
	static const int NUM_LIGHT_ATTRIBUTES = 12;

	GeneralLight light;

	// Get string representation of the int
	string numberStrg = static_cast<ostringstream*>(&(ostringstream() << lightNumber))->str();

	string uniformNames[NUM_LIGHT_ATTRIBUTES];

	GLuint uniformIndices[NUM_LIGHT_ATTRIBUTES] = { 0 };
	GLint uniformOffsets[NUM_LIGHT_ATTRIBUTES] = { 0 };

	// Get the indices of the uniform block variables in the shader
	static const string names[NUM_LIGHT_ATTRIBUTES]
		= { "ambientColor", "diffuseColor", "specularColor",
		"positionOrDirection", "spotDirection",
		"isSpot", "spotCutoffCos", "spotExponent",
		"constant", "linear", "quadratic", "enabled" };

	for (int i = 0; i < NUM_LIGHT_ATTRIBUTES; i++) {

		uniformNames[i] = "lights[" + numberStrg + "]." + names[i];

	}

	const GLchar * charStringNames[] = { uniformNames[0].c_str(), uniformNames[1].c_str(),
		uniformNames[2].c_str(), uniformNames[3].c_str(),
		uniformNames[4].c_str(), uniformNames[5].c_str(),
		uniformNames[6].c_str(), uniformNames[7].c_str(),
		uniformNames[8].c_str(), uniformNames[9].c_str(),
		uniformNames[10].c_str(), uniformNames[11].c_str() };

	cout << endl;

	glGetUniformIndices(shaderProgram, NUM_LIGHT_ATTRIBUTES, (const GLchar **)charStringNames, uniformIndices);

	for (int i = 0; i < NUM_LIGHT_ATTRIBUTES; i++) {

		cout << charStringNames[i] << " indice is " << uniformIndices[i] << endl;
	}

	cout << endl;

	glGetActiveUniformsiv(shaderProgram, NUM_LIGHT_ATTRIBUTES, uniformIndices, GL_UNIFORM_OFFSET, uniformOffsets);

	for (int i = 0; i < NUM_LIGHT_ATTRIBUTES; i++) {

		cout << charStringNames[i] << " byte offset is " << uniformOffsets[i] << endl;
	}

	// Light color locations
	light.ambientColorLoc = uniformOffsets[0];
	light.diffuseColorLoc = uniformOffsets[1];
	light.specularColorLoc = uniformOffsets[2];

	// Position and direction location
	light.positionOrDirectionLoc = uniformOffsets[3];

	// Spotlight locations
	light.spotDirectionLoc = uniformOffsets[4];
	light.isSpotLoc = uniformOffsets[5];
	light.spotCutoffCosLoc = uniformOffsets[6];
	light.spotExponentLoc = uniformOffsets[7];

	//Attenuation factor locations
	light.constantLoc = uniformOffsets[8];
	light.linearLoc = uniformOffsets[9];
	light.quadraticLoc = uniformOffsets[10];

	// Enabled location
	light.enabledLoc = uniformOffsets[11];

	return light;
}

void SharedGeneralLighting::initilizeAttributes(GLint lightNumber)
{
	setEnabled((lightSource)lightNumber, false);

	setAmbientColor((lightSource)lightNumber, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	setDiffuseColor((lightSource)lightNumber, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	setSpecularColor((lightSource)lightNumber, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	setPositionOrDirection((lightSource)lightNumber, glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));

	setIsSpot((lightSource)lightNumber, false);
	setSpotDirection((lightSource)lightNumber, glm::vec3(-1.0f, 1.0f, 0.0f));
	setSpotCutoffCos((lightSource)lightNumber, cos(glm::radians(180.0f)));
	setSpotExponent((lightSource)lightNumber, 50.0f);

	setConstantAttenuation((lightSource)lightNumber, 1.0f);
	setLinearAttenuation((lightSource)lightNumber, 0.0f);
	setQuadraticAttenuation((lightSource)lightNumber, 0.0f);
}


void SharedGeneralLighting::setEnabled(lightSource light, bool on)
{
	glBindBuffer(GL_UNIFORM_BUFFER, generalLightBlockBuffer);

	lights[light].enabled = on;

	int intOn = 0;
	if (lights[light].enabled) {
		intOn = 1;
	}

	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].enabledLoc, sizeof(int), &intOn);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setAmbientColor(lightSource light, glm::vec4 color4)
{
	glBindBuffer(GL_UNIFORM_BUFFER, generalLightBlockBuffer);

	lights[light].ambientColor = color4;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].ambientColorLoc, sizeof(glm::vec4), value_ptr(lights[light].ambientColor));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setDiffuseColor(lightSource light, glm::vec4 color4)
{
	glBindBuffer(GL_UNIFORM_BUFFER, generalLightBlockBuffer);

	lights[light].diffuseColor = color4;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].diffuseColorLoc, sizeof(glm::vec4), value_ptr(lights[light].diffuseColor));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setSpecularColor(lightSource light, glm::vec4 color4)
{
	glBindBuffer(GL_UNIFORM_BUFFER, generalLightBlockBuffer);

	lights[light].specularColor = color4;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].specularColorLoc, sizeof(glm::vec4), value_ptr(lights[light].specularColor));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setPositionOrDirection(lightSource light, glm::vec4 positOrDirect)
{
	glBindBuffer(GL_UNIFORM_BUFFER, generalLightBlockBuffer);

	lights[light].positionOrDirection = positOrDirect;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].positionOrDirectionLoc, sizeof(glm::vec4), value_ptr(lights[light].positionOrDirection));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setAttenuationFactors(lightSource light, glm::vec3 factors)
{
	setConstantAttenuation(light, factors.x);
	setLinearAttenuation(light, factors.y);
	setQuadraticAttenuation(light, factors.z);

}

void SharedGeneralLighting::setConstantAttenuation(lightSource light, float factor)
{
	glBindBuffer(GL_UNIFORM_BUFFER, generalLightBlockBuffer);

	lights[light].constant = factor;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].constantLoc, sizeof(float), &lights[light].constant);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setLinearAttenuation(lightSource light, float factor)
{
	glBindBuffer(GL_UNIFORM_BUFFER, generalLightBlockBuffer);

	lights[light].linear = factor;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].linearLoc, sizeof(float), &lights[light].linear);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setQuadraticAttenuation(lightSource light, float factor)
{
	glBindBuffer(GL_UNIFORM_BUFFER, generalLightBlockBuffer);

	lights[light].quadratic = factor;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].quadraticLoc, sizeof(float), &lights[light].quadratic);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setIsSpot(lightSource light, bool spotOn)
{
	glBindBuffer(GL_UNIFORM_BUFFER, generalLightBlockBuffer);

	lights[light].isSpot = spotOn;
	int intIsSpot = 0;
	if (lights[light].enabled) {
		intIsSpot = 1;
	}

	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].isSpotLoc, sizeof(int), &intIsSpot);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setSpotDirection(lightSource light, glm::vec3 spotDirect)
{
	glBindBuffer(GL_UNIFORM_BUFFER, generalLightBlockBuffer);

	lights[light].spotDirection = glm::normalize(spotDirect);
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].spotDirectionLoc, sizeof(glm::vec3), value_ptr(lights[light].spotDirection));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setSpotCutoffCos(lightSource light, float cutoffCos)
{
	glBindBuffer(GL_UNIFORM_BUFFER, generalLightBlockBuffer);

	lights[light].spotCutoffCos = cutoffCos;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].spotCutoffCosLoc, sizeof(float), &lights[light].spotCutoffCos);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setSpotExponent(lightSource light, float spotEx)
{
	glBindBuffer(GL_UNIFORM_BUFFER, generalLightBlockBuffer);

	lights[light].spotExponent = spotEx;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].spotExponentLoc, sizeof(float), &lights[light].spotExponent);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}