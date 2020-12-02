#version 410
layout (location = 0) in  vec4 vVertex;
layout (location = 1) in  vec4 vNormal;

out vec3 vertNormal;
out vec3[3] lightDir;
out vec3 eyeDir; 
//out int lightnum;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightPos[3];

void main() 
{
	vertNormal = normalize(normalMatrix * vNormal.xyz); /// Rotate the normal to the correct orientation 
	vec3 vertPos = vec3(viewMatrix * modelMatrix * vVertex); /// This is the position of the vertex from the origin
	vec3 vertDir = normalize(vertPos);
	eyeDir = -vertDir;

	for(int i = 0; i < 3; i++)
	{
		lightDir[i] = normalize(lightPos[i] - vertPos); /// Create the light direction. I do the math with in class 
	}
	

	gl_Position =  projectionMatrix * viewMatrix * modelMatrix * vVertex; 
}
