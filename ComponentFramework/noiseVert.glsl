#version 410
layout (location = 0) in  vec4 vVertex;
layout (location = 1) in  vec4 vNormal;
layout (location = 2) in  vec2 uvCoords;

out vec3 vertNormal;
out vec3 vertDir;
out vec3 vertPos;
out float fragTime;	
											 
uniform mat4 projectionMatrix;				 
uniform mat4 viewMatrix;					 
uniform mat4 modelMatrix;					 
uniform mat3 normalMatrix;	
uniform float time;		
uniform sampler3D noiseTexture;
					 
void main() {					

	fragTime = time;
	
	vertNormal = normalize(normalMatrix * vNormal.xyz); 
	
	vertPos = vec3(modelMatrix * vVertex + texture(noiseTexture, vVertex.xyz * 0.1 + time * 0.1) * 2.0); /// This is the position of the vertex from the origin
	vertDir = normalize(vertPos);
	
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertPos, 1.0);
}
