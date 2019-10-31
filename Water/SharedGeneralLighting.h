#pragma once

#include "BasicIncludesAndDefines.h"

#define generalLightBlockBindingPoint 22

// Maximum number of lights
// If this is increased, the enumerated type below needs to be extended and the 
// size of the lights array in the shader program needs to be adjusted.
#define MAX_LIGHTS 8

// Enumerated type for the names of the lights
enum lightSource {
	GL_LIGHT_ZERO, GL_LIGHT_ONE, GL_LIGHT_TWO, GL_LIGHT_THREE,
	GL_LIGHT_FOUR, GL_LIGHT_FIVE, GL_LIGHT_SIX, GL_LIGHT_SEVEN
};


// Structure for holding the attributes of an individual light source
struct GeneralLight {

	GLint ambientColorLoc; glm::vec4 ambientColor;		// ambient color of the light
	GLint diffuseColorLoc; glm::vec4 diffuseColor;		// diffuse color of the light
	GLint specularColorLoc; glm::vec4 specularColor;		// specular color of the light

	// Either the position or direction
	// if w = 0 then the light is directional
	// if w = 1 then the light is positional
	// direction is the negative of the direction the light is shinning
	GLint positionOrDirectionLoc; glm::vec4 positionOrDirection;

	// spotlight attributes
	GLint spotDirectionLoc; glm::vec3 spotDirection;		// the direction the cone of light is shinning      
	GLint isSpotLoc; bool isSpot;					// true if the light is a spotlight
	GLint spotCutoffCosLoc; float spotCutoffCos;	// Cosine of the spot cutoff angle
	GLint spotExponentLoc; float spotExponent;		// spot exponent for falloff calculation

	// attenuation coefficients
	GLint constantLoc; float constant;
	GLint linearLoc; float linear;
	GLint quadraticLoc; float quadratic;

	GLint enabledLoc; bool enabled;			// true if light is "on"
};

class SharedGeneralLighting
{
public:
	
	static void setUniformBlockForShader(GLuint shaderProgram);

	static bool getEnabled(lightSource light) { return lights[light].enabled; }
	static void setEnabled(lightSource light, bool on);

	static glm::vec4 getAmbientColor(lightSource light) { return lights[light].ambientColor; }
	static void setAmbientColor(lightSource light, glm::vec4 color4);

	static glm::vec4 getDiffuseColor(lightSource light) { return lights[light].diffuseColor; }
	static void setDiffuseColor(lightSource light, glm::vec4 color4);

	static glm::vec4 getSpecularColor(lightSource light) { return lights[light].specularColor; }
	static void setSpecularColor(lightSource light, glm::vec4 color4);

	static glm::vec4 getPositionOrDirection(lightSource light) { return lights[light].positionOrDirection; }
	static void setPositionOrDirection(lightSource light, glm::vec4 positOrDirect);

	static void setAttenuationFactors(lightSource light, glm::vec3 factors);

	static float getConstantAttenuation(lightSource light) { return lights[light].constant; }
	static void setConstantAttenuation(lightSource light, float factor);

	static float getLinearAttenuation(lightSource light) { return lights[light].linear; }
	static void setLinearAttenuation(lightSource light, float factor);

	static float getQuadraticAttenuation(lightSource light) { return lights[light].quadratic; }
	static void setQuadraticAttenuation(lightSource light, float factor);

	static bool getIsSpot(lightSource light) { return lights[light].isSpot; }
	static void setIsSpot(lightSource light, bool spotOn);

	static glm::vec3 getSpotDirection(lightSource light) { return lights[light].spotDirection; }
	static void setSpotDirection(lightSource light, glm::vec3 spotDirect);

	static float getSpotCutoffCos(lightSource light) { return lights[light].spotCutoffCos; }
	static void setSpotCutoffCos(lightSource light, float cutoffCos);

	static float getSpotExponent(lightSource light) { return lights[light].spotExponent; }
	static void setSpotExponent(lightSource light, float spotEx);

protected:

	// Finds the byte offsets of the variables in the two uniform 
	// blocks.
	static void findOffsets(GLuint shaderProgram);

	// Creates the buffers and binds them to the binding points.
	static void allocateBuffers(GLuint shaderProgram);

	// Determines the sizes in bytes of the blocks and binds them to the binding points. 
	static void determineBlockSizeSetBindingPoint(GLuint shaderProgram);

	static GeneralLight SharedGeneralLighting::initilizeLight(GLuint shaderProgram, GLint lightNumber);

	static void SharedGeneralLighting::initilizeAttributes(GLint lightNumber);

	static vector<GeneralLight> lights;

	static GLuint SharedGeneralLighting::generalLightBlockBuffer; // Identifier for the buffer. There would only be one buffer to feed several uniform blocks.
	static GLint SharedGeneralLighting::generalLightBlockSize; // Size in bytes of both the buffer and the uniform blocks in all the the shaders. 
	static GLuint SharedGeneralLighting::generalLightBlockIndex; // Identifier for the uniform block.

	static bool SharedGeneralLighting::blockSizeAndOffetsSet; // Indicates whether or not the buffer for the blocks has been setup

	const static string SharedGeneralLighting::generalLightBlockName;
};

