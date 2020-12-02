#version 410
in vec3 vertPos;

out vec4 fragColor;



void main() 
{
	fragColor = vec4(vertPos, 1.0);
}
