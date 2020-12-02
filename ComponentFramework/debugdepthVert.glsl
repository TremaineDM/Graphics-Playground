#version 410
layout (location = 0) in  vec4 vVertex;
layout (location = 1) in vec2 texC;

out vec2 texCoords;

void main()
{
	texCoords = texC;
	gl_Position =  vVertex;
}
