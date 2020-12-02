#version 410
in vec3 vertNormal;
in vec3 lightDir;
in vec3 vertDir;
in vec2 uvCoords;
in float fragTime;
out vec4 fragColor;

uniform sampler2D myTexture; 

void main() { 
	float spec = 0.0;
	float back = 1.0;
	const vec4 specLightColor = vec4(0.6, 0.6, 0.6, 1.0);
	float diff = max(dot(vertNormal, lightDir), 0.0);
	vec4 textureColor = texture(myTexture, uvCoords);
	if(diff > 0.2){
		vec3 reflection = normalize(reflect(lightDir, vertNormal));
		//spec = max(dot(vertDir, reflection), 0.0);
		//spec = pow(spec,40.0);
		if(textureColor.b > 0.1 && textureColor.g < 0.1)
		{
			spec = max(dot(vertDir, reflection), 0.0);
			spec = pow(spec, 20.0);
		}
	}

	vec4 ka = 0.02 * textureColor;

	fragColor = (ka + (diff * textureColor)  + (spec * specLightColor)); 
} 

