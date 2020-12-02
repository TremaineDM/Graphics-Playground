#version 410
layout (location = 0) in  vec4 vVertex;
layout (location = 1) in  vec4 vNormal;
layout (location = 2) in  vec2 texCoords;


out vec3 vertNormal;
out vec3 lightDir;
out vec3 vertDir; 
out vec2 uvCoords;
out float fragTime;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightPos;
uniform float time;

void main() {
	uvCoords = texCoords;
	fragTime = time;
	vertNormal = mat3(viewMatrix) * normalMatrix * vNormal.xyz; /// Rotate the normal to the correct orientation 
	float t = (cos((time * 10.0 ) + (vVertex.y * 5.0)) * 0.05 + 2.0) * 0.4;
	vec4 v = vec4(vVertex.x * t, vVertex.y * t, vVertex.z * t, vVertex.w);
	vec3 vertPos = vec3(viewMatrix * modelMatrix * v); /// Creates the vertex position (-eyeDir) 
	lightDir = normalize(lightPos +  -vertPos); /// Create the light direction 
	vertDir = normalize(vertPos);
	gl_Position =  projectionMatrix * viewMatrix * modelMatrix * v;
	gl_PointSize = 8.0;
}
