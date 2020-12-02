#version 410
layout (location = 0) in  vec4 vVertex;
layout (location = 1) in  vec4 vNormal;
layout (location = 2) in  vec2 uvCoords;


out Data 
{
	vec3 vertNorm;
	vec3 vertDir;
	vec3 eyeDir;
	vec3 vertPos;
	vec3 lightDir;

} dataOut;


uniform mat3 normalMatrix;							 
uniform mat4 projectionMatrix;				 
uniform mat4 viewMatrix;					 
uniform mat4 modelMatrix;					 
uniform vec3 lightPos;	
uniform sampler3D noiseTexture;

void main() {					
	dataOut.vertNorm = normalize(vNormal.xyz); // Rotate the normal to the correct orientation 
	dataOut.vertPos = vec3(vVertex); // This is the position of the vertex from the origin
	dataOut.vertDir = normalize(dataOut.vertPos);
	dataOut.eyeDir = vec3(normalize(vVertex));//rotated position of vertex from origin

	gl_Position;
}
