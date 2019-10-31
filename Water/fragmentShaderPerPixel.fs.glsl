#version 430 core

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
	int textureMode; // NO_TEXTURE = 0, DECAL = 1, REPLACE_AMBIENT_DIFFUSE = 2
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


// Samplers for texture mapping
uniform sampler2D ambient_sampler;
uniform sampler2D diffuse_sampler;
uniform sampler2D specular_sampler;

in vec4 vertexWorldPosition;
in vec4 vertexWorldNormal;
in vec2 TexCoord0;

out vec4 fragmentColor;


vec3 localIllumination(GeneralLight light, Material object )
{
	vec3 totalFromThisLight = vec3(0.0, 0.0, 0.0);

	// Caculated ambient, diffuse, and specular reflection for the light
	// and add in the emmissive color of the object.
	// The light may be positional or directional. 
	// Positional lights may be spot lights.

	if(light.enabled == 1){

		vec3 l;

		//if is directional
		if(light.positionOrDirection.w != 1){
		  l = light.positionOrDirection.xyz;
		}
		//is positional
		else{
		  l = normalize(light.positionOrDirection.xyz - vertexWorldPosition.xyz);
		}

		//ambient
		totalFromThisLight += light.ambientColor.xyz * object.ambientMat.xyz;

		//diffuse
		vec4 diff = max(0, dot(vertexWorldNormal.xyz, l)) * light.diffuseColor * object.diffuseMat;
		totalFromThisLight += diff.xyz;

		//specular
		vec3 viewVector = normalize(vertexWorldPosition.xyz - worldEyePosition);
		vec3 reflectionVector = normalize(reflect(l, vertexWorldNormal.xyz));
		vec4 spec = pow(max(0, dot(viewVector, reflectionVector)), object.specularExp) * light.specularColor * object.specularMat;
		totalFromThisLight += spec.xyz;

		if(light.isSpot == 1){
	        
		}
		
	}

	return totalFromThisLight;

} // end shadingCaculation



void main()
{
    fragmentColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    for(int i = 0; i < MaxLights; i++){
	  fragmentColor += vec4(localIllumination(lights[i], object), 0.1f);
	}
	fragmentColor.a = .5f;
	//fragmentColor = texture( diffuse_sampler, TexCoord0.st );

} // end main