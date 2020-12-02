#version 410
layout (vertices = 3) out;

in vec3 vertPos_CS[];
in vec3 vertNormal_CS[];
in vec2 uvCoords_CS[];

out vec3 vertPos_ES[];
out vec3 vertNormal_ES[];
out vec2 uvCoords_ES[];

uniform float fov;

void main() 
{
	vertPos_ES[gl_InvocationID] = vertPos_CS[gl_InvocationID];
	vertNormal_ES[gl_InvocationID] = vertNormal_CS[gl_InvocationID];
	uvCoords_ES[gl_InvocationID] = uvCoords_CS[gl_InvocationID];

	float f = fov / 10.0;

	gl_TessLevelOuter[0] = 3.0 * (1.0 / f);
	gl_TessLevelOuter[1] = 3.0 * (1.0 / f);
	gl_TessLevelOuter[2] = 3.0 * (1.0 / f);

	gl_TessLevelInner[0] = 9.0 * (1.0 / f);
}
