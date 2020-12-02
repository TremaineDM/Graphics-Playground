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
	const vec4 specLightColor = vec4(0.2, 0.2, 0.2, 1.0);
	vec4 rimColor = vec4(0.0, 0.0, 0.0, 1.0);
	float diff = max(dot(vertNormal, lightDir), 0.0);
	vec4 textureColor = texture(myTexture, uvCoords);
	if(diff > 0.0){
		vec3 reflection = normalize(reflect(lightDir, vertNormal));
		spec = max(dot(vertDir, reflection), 0.0);
		spec = pow(spec,40.0);
	}

	if(length(cross(vertDir, vertNormal)) > 0.95)
	{
		rimColor = vec4(abs(cos(fragTime) * 1.0), abs(sin(fragTime) * 1.0), cos(fragTime * 3.0) * 1.0, 1.0);
	}

	if(dot(vertDir, vertNormal) > -0.2)
	{
		back = 0.3;
	}
	vec4 ka = 0.05 * textureColor;
	textureColor.r = cos(fragTime * 10.0 + gl_FragCoord.y * 1.0)/2 + 1.0;
	//ka.r *= 5.0;
	fragColor =  ((ka + (diff * textureColor)  + (spec * specLightColor)) /* abs(cos(fragTime))*/ + (rimColor * back)); 	
	//fragColor = ka + vec4(vertNormal, 0.0);
} 

