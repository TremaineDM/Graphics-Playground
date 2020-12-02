#version 410
layout (location = 0) in  vec4 vVertex;
layout (location = 1) in  vec4 vNormal;
layout (location = 2) in  vec2 texCoords;

out vec3 vertPos;
out float Ftime;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform float time;

void main() {
	Ftime = time;
	vec3 vertNormal = normalize(normalMatrix * vNormal.xyz); /// Rotate the normal to the correct orientation 
	vertPos = (normalize(vec3(vNormal)) * 0.5) + 0.5; /// Creates the vertex position (-eyeDir) 
	//vertDir = normalize(-vertPos);
	gl_Position =  projectionMatrix * viewMatrix * modelMatrix * vVertex;
}
