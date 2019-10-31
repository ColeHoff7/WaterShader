// Targeting version 4.2 of GLSL. 
#version 330 core

#pragma optimize(off)
#pragma debug(on)

// Declare default precision - eliminates warnings 
precision mediump float;

layout (shared) uniform worldEyeBlock
{
	vec3 worldEyePosition;
};

struct Material
{
	vec4 ambientMat;
	vec4 diffuseMat;
	vec4 specularMat;
	vec4 emmissiveMat; // *new added emmissive property
	float specularExp;
	int textureMode;
	
};

layout (shared) uniform MaterialBlock
{
	Material object;
};

// Structure for holding general light properties
struct GeneralLight {
  
	vec4 ambientColor;		// ambient color of the light
	vec4 diffuseColor;		// diffuse color of the light
    vec4 specularColor;		// specular color of the light

	// Either the position or direction
	// if w = 0 then the light is directional and direction specified 
	// is the negative of the direction the light is shinning
	// if w = 1 then the light is positional
    vec4 positionOrDirection;    
					  
	// spotlight attributes
    vec3 spotDirection;		// the direction the cone of light is shinning    
	int isSpot;				// 1 if the light is a spotlight  
    float spotCutoffCos;	// Cosine of the spot cutoff angle
    float spotExponent;		// For gradual falloff near cone edge

	// attenuation coefficients
    float constant; 
    float linear;
    float quadratic;

	int enabled;			// 1 if light is "on"

};

const int MaxLights = 8;

layout (std140) uniform LightBlock
{
 GeneralLight lights[MaxLights];
};

in vec4 vertexWorldPosition;
in vec4 vertexWorldNormal;
in vec2 TexCoord0;


vec3 shadingCaculation(GeneralLight light, Material object )
{
	vec3 totalFromThisLight = vec3(0.0, 0.0, 0.0);

	if (light.enabled == 1) {

		// Calculate a bunch of vectors
		vec3 lightVector;
		if (light.positionOrDirection.w < 1) {
			// Directional
			lightVector = normalize(light.positionOrDirection.xyz);
		}
		else {
			// Positional
			lightVector = normalize(light.positionOrDirection.xyz - 
				vertexWorldPosition.xyz );
		}

		vec3 reflection = normalize( reflect(-lightVector, vertexWorldNormal.xyz) );
		vec3 eyeVector = normalize(worldEyePosition - vertexWorldPosition.xyz);

		float spotCosFactor = 0; 
		if (light.isSpot == 1) {
		
			spotCosFactor = dot (-lightVector, normalize(light.spotDirection));
		}	

		// Is it a spot light and are we in the cone?
		if( light.isSpot < 1 ||  (light.isSpot == 1 && spotCosFactor >= light.spotCutoffCos ) ) {

			// Ambient Reflection
			totalFromThisLight += object.ambientMat.xyz * light.ambientColor.xyz;

			// Difusse Reflection
			totalFromThisLight += max(dot(vertexWorldNormal.xyz, lightVector ), 0) *
				object.diffuseMat.xyz * light.diffuseColor.xyz;

			totalFromThisLight += pow( max( dot( reflection, eyeVector ), 0), object.specularExp) *
			object.specularMat.xyz * light.specularColor.xyz;
		}
	}

	return totalFromThisLight;

} // end shadingCaculation

out vec4 fragmentColor;

// Create a to sampler
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_bump1;

void main()
{
	switch (object.textureMode) {
	
		case 0:
			fragmentColor = object.emmissiveMat;

			for (int i = 0; i < MaxLights; i++)  {
	
				fragmentColor += vec4( shadingCaculation( lights[i], object ), 1.0);
			}

			break;

		case 1:
		 
			fragmentColor = texture2D(texture_diffuse1, TexCoord0.st);

			//fragmentColor = object.diffuseMat;
			break;

		case 2:
			
			Material illuminatedObject = object;

			vec4 textColor =  texture2D(texture_diffuse1, TexCoord0.st);
			illuminatedObject.ambientMat = textColor;
			illuminatedObject.diffuseMat = textColor;
			//illuminatedObject.specularMat = texture2D(texture_diffuse1, TexCoord0.st);

			fragmentColor = illuminatedObject.emmissiveMat;

			for (int i = 0; i < MaxLights; i++)  {
	
				fragmentColor += vec4( shadingCaculation( lights[i], illuminatedObject ), 1.0);
			}

			break;
	}

	//fragmentColor = object.ambientMat + object.diffuseMat + object.specularMat;

} // end main