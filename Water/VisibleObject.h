#pragma once

#include "BasicIncludesAndDefines.h"

//// Coordinate frames
//enum Frame { WORLD = 0, LOCAL };
//
//// Local directions
//static const glm::vec3 FORWARD(0, 0, -1);
//static const glm::vec3 RIGHT_SIDE(1, 0, 0);
//static const glm::vec3 UP(0, 1, 0);

class VisibleObject
{

public:

	VisibleObject();
	~VisibleObject();

	// Set the shader program for this object and its children
	void setShader(GLuint shaderProgram);

	// Set the texture mode for this object and its children
	void setTextureMode(TextureMode mode);

	/**
	* Initilizes "this" VisibleObject scene graph node
	* and all of its children.
	*/
	virtual void initialize();

	/**
	* Renders "this" VisibleObject scene graph node
	* and all of its children.
	*/
	virtual void draw();

	/**
	* Allows Behaviors to update "this" VisibleObject scene graph node
	* and all of its children.
	@param deltaTime time in seconds since the last update
	*/
	virtual bool update(float deltaTime); 

	/**
	* Returns the World transformation of this VisibleObject
	* scene graph node.
	* @return World transformation of this node
	*/
	glm::mat4 getModelTransformation();

	/**
	* Set the modeling transformation that will be used when this object is rendered
	* @param modelTrans 4 x 4 homogenous transformation matrix that describes the World transformation
	*/
	void setModelTransformation(glm::mat4 modelTrans);

	/**
	* Add a child to this VisibleObject scene graph node. Set the parent of
	* the child to "this" VisibleObject. Local transformation of the
	* child is unchanged.
	* @param visibleObject child that is being added as a child
	*/
	void addChild(VisibleObject* visibleObject);

	/**
	* Add a child to this VisibleObject scene graph node. Removed the child
	* from the children of the former parent. Local transformation is set
	* so that the position and orientation relative to the World reference
	* frame is unchanged. Set the parent of the child to "this" VisibleObject.
	* @param visibleObject child that is being added
	*/
	void reparent(VisibleObject* visibleObject);

	/**
	* Remove a child of this VisibleObject scene graph node. Parent of the
	* child is set to NULL.
	* @param visibleObject child that is being removed
	*/
	void removeChild(VisibleObject* visibleObject);

	/**
	* Detaches "this" VisibleObject scene graph node from its parent. Parent
	* is set to NULL.
	*/
	void detachFromParent();
		
	/**
	* Material properties of the object (public so that is can be adjusted
	* outside the class)
	*/
	Material material;


protected: 

	bool enabled; // true if the object should be rendered and updated

	GLuint shaderProgram; // Shader program used to render the object

	GLuint vertexArrayObject; // Vertex Fetch data for the object

	GLuint VBO; // Identifier for vertex buffer object

	unsigned int numberOfElements; // Number of vertices or indices

	glm::mat4 modelingTransformation;

	VisibleObject* parent; // Reference to parent in the scene graph

	vector<VisibleObject*> children; // Children of this VisibleObject

}; // end VisibleObject class

