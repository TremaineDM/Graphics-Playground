#version 410
layout (vertices = 3) out;

in Data 
{
	vec3 vertNorm;
	vec3 vertDir;
	vec3 eyeDir;
	vec3 vertPos;
	vec3 lightDir;

} dataIn[];

out Data 
{
	vec3 vertNorm;
	vec3 vertDir;
	vec3 eyeDir;
	vec3 vertPos;
	vec3 lightDir;

} dataOut[];


void main() 
{
	dataOut[gl_InvocationID].vertPos = dataIn[gl_InvocationID].vertPos;
	dataOut[gl_InvocationID].vertNorm = dataIn[gl_InvocationID].vertNorm;
	dataOut[gl_InvocationID].vertDir = dataIn[gl_InvocationID].vertDir;
	dataOut[gl_InvocationID].eyeDir = dataIn[gl_InvocationID].eyeDir;
	dataOut[gl_InvocationID].lightDir = dataIn[gl_InvocationID].lightDir;

	gl_TessLevelOuter[0] = 2.0;
	gl_TessLevelOuter[1] = 2.0;
	gl_TessLevelOuter[2] = 2.0;

	gl_TessLevelInner[0] = 5.0;
}
