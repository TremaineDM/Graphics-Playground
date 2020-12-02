#version 410
layout (location = 0) in  vec4 vVertex;
layout (location = 1) in  vec4 vNormal;
layout (location = 2) in  vec2 texCoords;

out vec3 vertPos_CS;
out vec3 vertNormal_CS;
out vec2 uvCoords_CS;

uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

void main() {

	vertNormal_CS = normalize(normalMatrix * vNormal.xyz); /// Rotate the normal to the correct orientation 
	uvCoords_CS = texCoords;
	vertPos_CS = vec3(modelMatrix * vVertex); /// Creates the vertex position (-eyeDir) 

}
