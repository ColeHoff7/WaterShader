#include "SharedProjectionAndViewing.h"

GLuint SharedProjectionAndViewing::projectionLocation; // Byte offset of the projection matrix
glm::mat4 SharedProjectionAndViewing::projectionMatrix; // Current projection matrix that is held in the buffer

GLuint SharedProjectionAndViewing::viewLocation; // Byte offset of the viewing matrix
glm::mat4 SharedProjectionAndViewing::viewMatrix; // Current viewing matrix that is held in the buffer

GLuint SharedProjectionAndViewing::modelLocation; // Byte offset of the modeling matrix
glm::mat4 SharedProjectionAndViewing::modelMatrix; // Current modeling matrix that is held in the buffer

GLuint SharedProjectionAndViewing::normalModelLocation; // Byte offset of the modeling matrix to transform normal vectors

GLuint SharedProjectionAndViewing::eyePositionLocation;  // Byte offset of the eye position

GLuint SharedProjectionAndViewing::projViewBuffer; // Identifier for the buffer. There would only be one buffer to feed several uniform blocks.
GLint SharedProjectionAndViewing::projViewBlockSize = 0; // Size in bytes of both the buffer and the uniform blocks in all the the shaders. 
GLuint SharedProjectionAndViewing::projViewBlockIndex; // Identifier for the uniform block.

GLuint SharedProjectionAndViewing::worldEyeBuffer; // Identifier for the buffer. There would only be one buffer to feed several uniform blocks.
GLint SharedProjectionAndViewing::worldEyeBlockSize = 0; // Size in bytes of both the buffer and the uniform blocks in all the the shaders. 
GLuint SharedProjectionAndViewing::worldEyeBlockIndex; // Identifier for the uniform block. 

bool SharedProjectionAndViewing::blockSizeAndOffetsSet = false; // Indicates whether or not the buffer for the blocks has been setup

const string SharedProjectionAndViewing::transformBlockName = "transformBlock";

const string SharedProjectionAndViewing::eyeBlockName = "worldEyeBlock";


bool checkLocationFound(const GLchar* locationName, GLuint indice )
{
	if (indice == GL_INVALID_INDEX ) {
		cout << locationName << " not found in shader." << endl;

		return false;
	}
	else {

		//cout << locationName << " index is " <<  indice << endl; 
		return true;
	}

} // end checkLocationFound


void SharedProjectionAndViewing::setUniformBlockForShader(GLuint shaderProgram)
{
	determineBlockSizeSetBindingPoint(shaderProgram);

	if ( blockSizeAndOffetsSet == false ) {

		// Set up the buffers and bind to binding points
		allocateBuffers( shaderProgram );

		findOffsets(shaderProgram);
	}

	checkOpenGLErrors("setUniformBlockForShader");

} // end setUniformBlockForShader


void SharedProjectionAndViewing::allocateBuffers(GLuint shaderProgram)
{
	if ( projViewBlockSize > 0 ) {

		// Get an identifier for a buffer
		glGenBuffers(1, &projViewBuffer);

		glBindBuffer(GL_UNIFORM_BUFFER, projViewBuffer);
 
		// Allocate the buffer. Does not load data.
		glBufferData(GL_UNIFORM_BUFFER, projViewBlockSize, NULL, GL_DYNAMIC_DRAW);

		// Assign the buffer to a binding point, 1 here
		glBindBufferBase(GL_UNIFORM_BUFFER, projectionViewBlockBindingPoint, projViewBuffer);

		blockSizeAndOffetsSet = true;
	}
		
	if ( worldEyeBlockSize > 0 ) {

		glGenBuffers(1, &worldEyeBuffer);

		glBindBuffer(GL_UNIFORM_BUFFER, worldEyeBuffer);
 
		// Allocate the buffer
		glBufferData(GL_UNIFORM_BUFFER, worldEyeBlockSize, NULL, GL_DYNAMIC_DRAW);

		// Assign the buffer to a binding point 
		glBindBufferBase(GL_UNIFORM_BUFFER, worldEyeBlockBindingPoint, worldEyeBuffer);

		blockSizeAndOffetsSet = true;
	}

	checkOpenGLErrors("findOffets");

} // end allocateBuffers


void SharedProjectionAndViewing::findOffsets(GLuint shaderProgram)
{
	const int numberOfNames = 4;

	GLuint uniformIndices[numberOfNames] = { 0 };
	GLint uniformOffsets[numberOfNames] = { 0 };

	const GLchar * charStringNames[] = { "modelMatrix", "viewingMatrix", "projectionMatrix", "normalModelMatrix" };

	glGetUniformIndices(shaderProgram, numberOfNames, (const GLchar **)charStringNames, uniformIndices);

	if (uniformIndices[0] != GL_INVALID_INDEX && uniformIndices[1] != GL_INVALID_INDEX) {
		for (int i = 0; i < numberOfNames; i++) {

			checkLocationFound(charStringNames[i], uniformIndices[i]);
		}

		//Get the offsets of the uniforms. The offsets in the buffer will be the same.
		glGetActiveUniformsiv(shaderProgram, numberOfNames, uniformIndices, GL_UNIFORM_OFFSET, uniformOffsets);

		for (int i = 0; i < numberOfNames; i++) {

			cout << '\t' << charStringNames[i] << " offset is " << uniformOffsets[i] << endl;
		}

		// Save locations
		modelLocation = uniformOffsets[0];
		viewLocation = uniformOffsets[1];
		projectionLocation = uniformOffsets[2];
		normalModelLocation = uniformOffsets[3];
	}
	else {

		cout << " Did not find names in " << transformBlockName.c_str() << endl;
	}

	GLuint uniformEyeIndice = 0;
	GLint uniformEyeOffset = 0;

	const GLchar * eyeName[] = { "worldEyePosition" };

	glGetUniformIndices(shaderProgram, 1, eyeName, &uniformEyeIndice);

	if (uniformEyeIndice != GL_INVALID_INDEX) {

		//Get the offsets of the uniforms
		glGetActiveUniformsiv(shaderProgram, 1, &uniformEyeIndice, GL_UNIFORM_OFFSET, &uniformEyeOffset);

		cout << '\t' << eyeName[0] << " offset is " << uniformEyeOffset << endl;

		// Save location
		eyePositionLocation = uniformEyeOffset;
	}
	else {

		cout << " Did not find names in " << eyeBlockName.c_str() << endl;
	}

	checkOpenGLErrors("findOffets");

} // findOffsets


void SharedProjectionAndViewing::determineBlockSizeSetBindingPoint( GLuint shaderProgram )
{
	projViewBlockIndex = glGetUniformBlockIndex(shaderProgram, transformBlockName.c_str());

	cout << transformBlockName.c_str() << " index is " << projViewBlockIndex << endl;

	if (checkLocationFound(transformBlockName.c_str(), projViewBlockIndex)) {

		// Determine the size in bytes
		glGetActiveUniformBlockiv(shaderProgram, projViewBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &projViewBlockSize);
		cout << transformBlockName.c_str() << " size is " << projViewBlockSize << endl;

		glUniformBlockBinding(shaderProgram, projViewBlockIndex, projectionViewBlockBindingPoint);
	}


	worldEyeBlockIndex = glGetUniformBlockIndex(shaderProgram, eyeBlockName.c_str() ); 
	cout << eyeBlockName.c_str() << " index is " << worldEyeBlockIndex << endl;

	if ( checkLocationFound( "worldEyeBlock", worldEyeBlockIndex  ) ) {

		glGetActiveUniformBlockiv(shaderProgram, worldEyeBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &worldEyeBlockSize);
		cout << eyeBlockName.c_str() << " size is " << worldEyeBlockSize << endl;

		glUniformBlockBinding(shaderProgram, worldEyeBlockIndex, worldEyeBlockBindingPoint);
	}

	checkOpenGLErrors("findBlockSizeSetBindingPoint");

} // end determineBlockSizeSetBindingPoint


void SharedProjectionAndViewing::setViewMatrix( glm::mat4 viewMatrix)
{
	if (projViewBlockSize > 0  ) {

		glBindBuffer(GL_UNIFORM_BUFFER, projViewBuffer);

		viewMatrix = viewMatrix;

		glBufferSubData(GL_UNIFORM_BUFFER, viewLocation, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
	}

	if (worldEyeBlockSize > 0 ) {

		glBindBuffer(GL_UNIFORM_BUFFER, worldEyeBuffer);

		glm::vec3 viewPoint = (glm::inverse(viewMatrix)[3]).xyz;

		glBufferSubData(GL_UNIFORM_BUFFER, eyePositionLocation, sizeof(glm::vec3), glm::value_ptr(viewPoint));
	}

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	checkOpenGLErrors("setViewMatrix");

} // end setViewMatrix


// Accessor for the current viewing matrix
glm::mat4 SharedProjectionAndViewing::getViewMatrix()
{
	return viewMatrix;

} // end getViewMatrix


void SharedProjectionAndViewing::setProjectionMatrix( glm::mat4 projectionMatrix)
{

	if (projViewBlockSize > 0  ) {
	
		// Bind the buffer. 
		glBindBuffer(GL_UNIFORM_BUFFER, projViewBuffer);

		projectionMatrix = projectionMatrix;

		glBufferSubData(GL_UNIFORM_BUFFER, projectionLocation, sizeof(glm::mat4), glm::value_ptr(projectionMatrix));

		// Unbind the buffer. 
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	checkOpenGLErrors("setProjectionMatrix");

} // end setProjectionMatrix


// Accessor for the current projection matrix
glm::mat4 SharedProjectionAndViewing::getProjectionMatrix()
{
	return projectionMatrix;

} // end getProjectionMatrix


void SharedProjectionAndViewing::setModelingMatrix(glm::mat4 modelingMatrix)
{
	checkOpenGLErrors("setModelingMatrix0");

	if (projViewBlockSize > 0) {

		// Bind the buffer. 
		glBindBuffer(GL_UNIFORM_BUFFER, projViewBuffer);

		modelMatrix = modelingMatrix;

		checkOpenGLErrors("setModelingMatrix1");

		glBufferSubData(GL_UNIFORM_BUFFER, modelLocation, sizeof(glm::mat4), glm::value_ptr(modelMatrix));

		glBufferSubData(GL_UNIFORM_BUFFER, normalModelLocation, sizeof(glm::mat4), 
			glm::value_ptr(glm::transpose(glm::inverse(modelMatrix))));

		// Unbind the buffer. 
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	checkOpenGLErrors("setModelingMatrix2");

} // end setModelingMatrix


// Accessor for the current modeling matrix
glm::mat4 SharedProjectionAndViewing::getModelingMatrix()
{
	return modelMatrix;

} // end getModelingMatrix

