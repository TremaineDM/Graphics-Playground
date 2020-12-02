#version 410

in vec3 vertPos;
in float Ftime;

uniform sampler3D noiseTex;
out vec4 fragColor;

void main() 
{ 

	float x = sin(Ftime + vertPos.x) * 0.5;
	float y = sin(Ftime + vertPos.y) * 0.5;
	float z = sin(Ftime + vertPos.z) * 0.5;

	mat4 cMat = mat4
	(
		vertPos.x,	0.0,		0.0,	   0.0,
		0.0,		vertPos.y,	0.0,	   0.0,
		0.0,		0.0,		vertPos.z, 0.0,
		0.0,		0.0,		0.0,	   1.0
	);

	vec4 noiseVec = texture(noiseTex, vertPos + (Ftime * 0.2));
	//vec4 noiseVec = noise4(vertPos);

	noiseVec *= 1.5;

	fragColor = noiseVec + cMat * vec4(vertPos, 1.0);
} 

