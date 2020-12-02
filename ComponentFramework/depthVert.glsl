#version 410
layout (location = 0) in  vec4 vVertex;


uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
	gl_Position =  lightSpaceMatrix * model * vVertex;
}
