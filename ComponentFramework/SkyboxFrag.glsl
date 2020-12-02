#version 410


out vec4 fragColor;

in vec3 texCoords;


uniform samplerCube skybox; 

void main() 
{ 
	vec4 boxColour = texture(skybox, texCoords);
	//boxColour *= (texCoords.y + 1.5) * 0.5;
	
	if(boxColour.r > 0.7 && boxColour.g > 0.7)
	{
		//boxColour.a = 0.3;
	}

	fragColor = boxColour;

} 

