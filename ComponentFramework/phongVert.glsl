#version 410
layout (location = 0) in  vec4 vVertex;
layout (location = 1) in  vec4 vNormal;
layout (location = 2) in  vec2 uvCoords;

out vec3 vertNormal;
out vec3 lightDir;
out vec3 eyeDir; 
out vec2 texCoords;
out vec4 lightSpacePos;
out vec3 lightPosition;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 lightSpaceMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightPos;



void main() 
{
	texCoords = uvCoords;
	vertNormal = normalize(normalMatrix * vNormal.xyz); /// Rotate the normal to the correct orientation 
	vec3 vertPos = vec3(modelMatrix * vVertex); /// This is the position of the vertex from the origin
	vec3 vertDir = normalize(vec3(viewMatrix * vec4(vertPos, 1.0)));
	eyeDir = -vertDir;

	lightSpacePos = lightSpaceMatrix * modelMatrix * vVertex;
	lightPosition = lightPos;
	lightDir = normalize(lightPos - vertPos); /// Create the light direction. I do the math with in class 
	gl_Position =  projectionMatrix * viewMatrix * modelMatrix * vVertex; 
}
