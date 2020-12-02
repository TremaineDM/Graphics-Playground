#version 410

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D depthmap;


void main() 
{ 
	float d  = texture(depthmap, texCoords).r;
	fragColor = vec4(vec3(d), 1.0);
} 

