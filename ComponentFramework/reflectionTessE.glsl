#version 410
layout (triangles, equal_spacing, ccw) in;

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

} dataOut;

uniform mat3 normalMatrix;							 
uniform mat4 projectionMatrix;				 
uniform mat4 viewMatrix;					 
uniform mat4 modelMatrix;					 
uniform vec3 lightPos;
uniform float time;		
uniform sampler3D noiseTexture;

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
} 

void main()
{
	dataOut.vertPos = interpolate3D(dataIn[0].vertPos, dataIn[1].vertPos, dataIn[2].vertPos);
    dataOut.vertNorm = interpolate3D(dataIn[0].vertNorm, dataIn[1].vertNorm, dataIn[2].vertNorm);
    dataOut.vertDir = interpolate3D(dataIn[0].vertDir, dataIn[1].vertDir, dataIn[2].vertDir);
    dataOut.eyeDir = interpolate3D(dataIn[0].eyeDir, dataIn[1].eyeDir, dataIn[2].eyeDir);
    dataOut.lightDir = interpolate3D(dataIn[0].lightDir, dataIn[1].lightDir, dataIn[2].lightDir);

	dataOut.vertNorm = normalize(normalMatrix * dataOut.vertNorm); // Rotate the normal to the correct orientation 
	dataOut.eyeDir = vec3(normalize(viewMatrix * modelMatrix * vec4(dataOut.vertPos, 1.0)));//rotated position of vertex from origin
	dataOut.vertPos = vec3(modelMatrix * vec4(dataOut.vertPos, 1.0)); // This is the position of the vertex from the origin
	vec3 noiseVec = vec3(texture(noiseTexture, dataOut.vertPos * 0.3 + time * 0.2) * 3.0);
	dataOut.vertPos += vec3(noiseVec);
	dataOut.vertDir = normalize(dataOut.vertPos);

	dataOut.lightDir = normalize(lightPos - dataOut.vertPos);
	
	gl_Position = projectionMatrix * viewMatrix * vec4(dataOut.vertPos, 1.0);
}
