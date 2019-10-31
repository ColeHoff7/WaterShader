#include "VisibleObject.h"

VisibleObject::VisibleObject()
	: parent(NULL), numberOfElements(0),
	modelingTransformation(glm::mat4(1.0f))
{

}


// Delete all the children as well as any associated vertex array object and vertex buffer.
VisibleObject::~VisibleObject()
{
	for (VisibleObject* obj : children) {

		delete obj;
	}

	children.clear();

	glDeleteVertexArrays(1, &vertexArrayObject);

	glDeleteBuffers(1, &VBO);

} // end destructor


  // Initilaize all the children of this VisibleObject.
void VisibleObject::initialize()
{
	for (unsigned int i = 0; i < children.size(); i++) {

		children[i]->initialize();
	}

	checkOpenGLErrors("VisibleObject::initialize");

} // end initialize


  // Render all the children of this VisibleObject
void VisibleObject::draw()
{
	for (unsigned int i = 0; i < children.size(); i++) {

		children[i]->draw();
	}

	checkOpenGLErrors("VisibleObject::draw");

} // end draw


  // Update this object and all its children. Delete any
  // children for which false is returned by their update
  // method.
bool VisibleObject::update(float deltaTime)
{
	size_t i = 0;

	while (i < children.size()) {

		if (!children[i]->update(deltaTime)) {

			delete children[i];

			children.erase(children.begin() + i);
		}
		else {
			i++;
		}

	}

	checkOpenGLErrors("VisibleObject::update");

	return true;

} // end update


  // Set the shader for this VisibleObject and all of its
  // children.
void VisibleObject::setShader(GLuint shaderProgram)
{
	this->shaderProgram = shaderProgram;

	for (VisibleObject* obj : children) {

		obj->setShader(shaderProgram);
	}

} // end setShader


  // Set the texture mode for this VisibleObject and all
  // of its children.
void VisibleObject::setTextureMode(TextureMode mode)
{
	this->material.setTextureMode(mode);

	for (VisibleObject* obj : children) {

		obj->setTextureMode(mode);
	}

} // end setTextureMode


// Set the modeling transformation that will be used when this object is rendered
void VisibleObject::setModelTransformation(glm::mat4 modelTrans)
{
	// Need to update the orientation and position.
	modelingTransformation = modelTrans;

} // end setModelTransformation

  // Get the transformation of this VisibleObject relative to the parent. i.e. the
  // local transformation.
glm::mat4 VisibleObject::getModelTransformation()
{
	return modelingTransformation;

} // end getLocalTransformation


  // Add a new child to this VisibleObject. Insure the child is
  // not attached to another parent.
void VisibleObject::addChild(VisibleObject* visibleObject)
{
	visibleObject->detachFromParent();

	children.push_back(visibleObject);

	visibleObject->parent = this;

} // end addChild


  // Attach a new child without changing its World coordinate position 
  // and orientation.
void VisibleObject::reparent(VisibleObject* visibleObject)
{
	glm::mat4 oldWorld = visibleObject->getModelTransformation();

	visibleObject->detachFromParent();

	addChild(visibleObject);

	visibleObject->modelingTransformation = glm::inverse(getModelTransformation()) * oldWorld;

} // end reparent


  // Remove a child from this VisibleObject
void VisibleObject::removeChild(VisibleObject* visibleObject)
{
	for (unsigned int i = 0; i < children.size(); i++) {

		if (children[i] == visibleObject) {

			children.erase(children.begin() + i);
			visibleObject->parent = NULL;
			break;
		}
	}

} // end removeChild


  // Detach this VisibleObject from its parent.
void VisibleObject::detachFromParent()
{
	if (parent != NULL) {

		parent->removeChild(this);
	}

} // end detachFromParent
