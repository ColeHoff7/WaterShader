#include "VisbleObjectIndexedRender.h"


VisbleObjectIndexedRender::VisbleObjectIndexedRender()
{
}


VisbleObjectIndexedRender::~VisbleObjectIndexedRender()
{
	glDeleteBuffers(1, &IBO);
}

void VisbleObjectIndexedRender::draw()
{
	// Bind vertex array object
	glBindVertexArray(vertexArrayObject);

	// Use the shader program
	glUseProgram(shaderProgram);

	// Set the modeling transformation
	SharedProjectionAndViewing::setModelingMatrix( this->getModelTransformation() );

	SharedMaterialProperties::setShaderMaterialProperties(shaderProgram, material);
	
	// Draw the object
	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);

	// Unbind the vertex array object
	glBindVertexArray(0);
	
	// Draw the kids
	VisibleObject::draw();

} // end draw
