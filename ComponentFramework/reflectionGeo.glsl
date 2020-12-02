#version 410
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

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


void main() 
{
	vec3 p1 = dataIn[0].vertPos - dataIn[1].vertPos;
	vec3 p2 = dataIn[0].vertPos - dataIn[2].vertPos;

	vec3 vertNormal = normalize(cross(p1, p2));
	
	dataOut.vertNorm = normalize(vertNormal + dataIn[0].vertNorm);
	//dataOut.vertNorm = dataIn[0].vertNorm;
	dataOut.vertPos = dataIn[0].vertPos;
	dataOut.vertDir = dataIn[0].vertDir;
	dataOut.eyeDir = dataIn[0].eyeDir;
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	
	dataOut.vertNorm = normalize(vertNormal + dataIn[1].vertNorm);
	//dataOut.vertNorm = dataIn[1].vertNorm;
	dataOut.vertPos = dataIn[1].vertPos;
	dataOut.vertDir = dataIn[1].vertDir;
	dataOut.eyeDir = dataIn[1].eyeDir;
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();
	
	dataOut.vertNorm = normalize(vertNormal + dataIn[2].vertNorm);
	//dataOut.vertNorm = dataIn[2].vertNorm;
	dataOut.vertPos = dataIn[2].vertPos;
	dataOut.vertDir = dataIn[2].vertDir;
	dataOut.eyeDir = dataIn[2].eyeDir;
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();


	EndPrimitive();

}
