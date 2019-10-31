#ifndef _SHARED_PROJECTION_AND_VIEWING_H_
#define	_SHARED_PROJECTION_AND_VIEWING_H_

#include "BasicIncludesAndDefines.h"

#define projectionViewBlockBindingPoint 2
#define worldEyeBlockBindingPoint 3

/**

static class that supports working with the two uniform blocks shown below.

setUniformBlockForShader should be called for every shader program that includes
the uniform block below. This will create a buffers for each uniform block
and bind the buffers and uniform blocks together through the binding points.
Binding points for the two blocks and the associated buffers are defined above.

Adding a #include for this header file makes the functionality avaible through
the class name.

layout(shared) uniform transformBlock
{
	mat4 modelMatrix;
	mat4 normalModelMatrix;
	mat4 viewingMatrix;
	mat4 projectionMatrix;
};

layout (shared) uniform worldEyeBlock
{
	vec3 worldEyePosition;
};

*/

class SharedProjectionAndViewing
{
	public:

	// Should be called for each shader program that includes the
	// projectionViewBlock and worldEyeBlock uniform blocks.
	static void setUniformBlockForShader(GLuint shaderProgram);

	// Accessor for the current projection matrix
	static glm::mat4 getProjectionMatrix();

	// Mutator for the projection matrix. Sets the projection
	// matrix in the buffer.
	static void setProjectionMatrix(glm::mat4 projectionMatrix);

	// Accessor for the current viewing matrix
	static glm::mat4 getViewMatrix();

	// Mutator for the viewing matrix. Sets the viewing
	// matrix in the buffer. Also sets the world eye position
	static void setViewMatrix(glm::mat4 viewMatrix);

	// Accessor for the current modeling matrix
	static glm::mat4 getModelingMatrix();

	// Mutator for the modeling matrix. Sets the modeling transformation
	// for both the vertex positions and normals in the buffer. 
	static void setModelingMatrix(glm::mat4 modelingMatrix);

	protected:

	// Finds the byte offsets of the variables in the two uniform 
	// blocks.
	static void findOffsets(GLuint shaderProgram);

	// Creates the buffers and binds them to the binding points.
	static void allocateBuffers(GLuint shaderProgram);

	// Determines the sizes in bytes of the blocks and binds them to the binding points. 
	static void determineBlockSizeSetBindingPoint(GLuint shaderProgram);

	static GLuint projectionLocation; // Byte offset of the projection matrix
	static glm::mat4 projectionMatrix; // Current projection matrix that is held in the buffer

	static GLuint viewLocation; // Byte offset of the viewing matrix
	static glm::mat4 viewMatrix; // Current viewing matrix that is held in the buffer

	static GLuint modelLocation; // Byte offset of the modeling matrix
	static glm::mat4 modelMatrix; // Current modeling matrix that is held in the buffer

	static GLuint normalModelLocation; // Byte offset of the modeling matrix to transform normal vectors

	static GLuint eyePositionLocation;  // Byte offset of the eye position

	static GLuint projViewBuffer; // Identifier for the buffer. There would only be one buffer to feed several uniform blocks.
	static GLint projViewBlockSize; // Size in bytes of both the buffer and the uniform blocks in all the the shaders. 
	static GLuint projViewBlockIndex; // Identifier for the uniform block.

	static GLuint worldEyeBuffer; // Identifier for the buffer. There would only be one buffer to feed several uniform blocks.
	static GLint worldEyeBlockSize; // Size in bytes of both the buffer and the uniform blocks in all the the shaders. 
	static GLuint worldEyeBlockIndex; // Identifier for the uniform block. 

	static bool blockSizeAndOffetsSet; // Indicates whether or not the buffer for the blocks has been setup

	static const string transformBlockName;

	static const string eyeBlockName;

}; // end SharedProjectionAndViewing class

#endif // _SHARED_PROJECTION_AND_VIEWING_H_

