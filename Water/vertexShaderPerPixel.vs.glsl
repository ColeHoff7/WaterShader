#version 430 core

#pragma optimize(off)
#pragma debug(on)

layout(shared) uniform transformBlock
{
	mat4 modelMatrix;
	mat4 normalModelMatrix;
	mat4 viewingMatrix;
	mat4 projectionMatrix;
};
const float pi = 3.14159;
const float waterHeight = 1.0f;
uniform float time;
const int numWaves = 3;
float amplitude[3];
float wavelength[3];
float speed[3];
float Q = .7f;
vec2 direction[3];


/*varying vec3 position;
varying vec3 worldNormal;
varying vec3 eyeNormal;
*/

layout (location = 0) in vec4 vertexPosition; 
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;

out vec4 vertexWorldPosition;
out vec4 vertexWorldNormal;
out vec2 TexCoord0;

float wave(int i, float x, float y) {
    float frequency = 2*pi/wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    return amplitude[i] * sin(theta * frequency + time * phase);
}

vec3 gerstnerWave(float x, float z){
	vec3 p = vec3(0.0f,0.0f,0.0f);
	for(int i = 0; i < numWaves; i++){
		float frequency = 2*pi/wavelength[i];
		float phase = speed[i] * frequency;
		p+=vec3(
			Q*amplitude[i] * direction[i].x * cos(dot(wavelength[i]*direction[i], vec2(x,z)) +  (phase * time)),
			amplitude[i]*sin(dot(wavelength[i]*direction[i], vec2(x,z)) +  (phase * time)),
			Q*amplitude[i] * direction[i].y * cos(dot(wavelength[i]*direction[i], vec2(x,z)) +  (phase * time))
		);
	}
	p.x += x;
	p.z += z;
	return p;
}

vec3 gerstnerNormal(float x, float z, vec3 p){
	vec3 n = vec3(0,0,0);
	for(int i = 0; i < numWaves; i++){
		float frequency = 2*pi/wavelength[i];
		float phase = speed[i] * frequency;
		float s = sin(wavelength[i]*dot(direction[i], p.xz) +  (phase * time));
		float c = cos(wavelength[i]*dot(direction[i], p.xz) +  (phase * time));
		n+= vec3(
			direction[i].x*wavelength[i]*amplitude[i] * c,
			Q*wavelength[i]*amplitude[i]*s,
			direction[i].y*wavelength[i]*amplitude[i] * c
		);
	}
	n.x = -n.x;
	n.z = -n.z;
	n.y = 1-n.y;
	return normalize(n);
}

float waveHeight(float x, float z) {
    float height = 0.0;
    for (int i = 0; i < numWaves; ++i)
        height += wave(i, x, z);
    return height;
}

float dWavedx(int i, float x, float z) {
    float frequency = 2*pi/wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, z));
    float A = amplitude[i] * direction[i].x * frequency;
    return A * cos(theta * frequency + time * phase);
}

float dWavedz(int i, float x, float z) {
    float frequency = 2*pi/wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, z));
    float A = amplitude[i] * direction[i].y * frequency;
    return A * cos(theta * frequency + time * phase);
}

vec3 waveNormal(float x, float z) {
    float dx = 0.0;
    float dz = 0.0;
    for (int i = 0; i < numWaves; ++i) {
        dx += dWavedx(i, x, z);
        dz += dWavedz(i, x, z);
    }
    vec3 n = vec3(-dx, -dz, 1.0f);
    return normalize(n);
}

void main() {
	//initialize wave parameter values
	for (int i = 0; i < 3; ++i) {
        float amp = 0.5f / (i + 1);
        amplitude[i] = amp*2;

        float wave = 8 * pi / (i + 1);
		wave = 8 * pi / (i + 1);
        wavelength[i] = wave;

        float sp = 1.0f + 2*i;
        speed[i] = sp/1000.0f;

       
    }
	float ang1 = -pi/2;
	float ang2 = pi/2;
	float ang3 = pi/5;
	direction[0] = vec2(cos(ang1), sin(ang1));
	direction[1] = vec2(cos(ang2), sin(ang2));
	direction[2] = vec2(cos(ang3), sin(ang3));

	vec4 pos = modelMatrix*vertexPosition;
	pos.y = waveHeight(pos.x, pos.z);
	//vec3 gerst = gerstnerWave(pos.x, pos.z);
	//pos = vec4(gerst, pos.w);
	vertexWorldPosition = pos;
	vertexWorldNormal = normalModelMatrix * vec4(vertexNormal, 1.0);
	vec3 n = waveNormal(vertexWorldPosition.x, vertexWorldPosition.z);
	//vec3 n = gerstnerNormal(vertexWorldPosition.x, vertexWorldPosition.z, gerst);
	vertexWorldNormal.x = n.x;
	vertexWorldNormal.y = n.y;
	vertexWorldNormal.z = n.z;
	TexCoord0 = vertexTexCoord; 
	gl_Position = projectionMatrix * viewingMatrix * modelMatrix * vertexWorldPosition;
}
