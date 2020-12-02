#version 410
in  vec3 vertNormal;
in  vec3 lightDir;
in  vec3 eyeDir;
in vec4 lightSpacePos;
in vec2 texCoords;
in vec3 lightPosition;



uniform sampler2D depthMap;
uniform mat4 viewMatrix;

out vec4 fragColor;

void main() { 
	vec4 ks = vec4(0.6, 0.6, 0.6, 1.0);
	const vec4 kd = vec4(0.4, 0.4, 0.4, 1.0); /// const means it cannot be changed just like C++
	const vec4 ka = vec4(vec3(0.5 * kd), 1.0);
	float diff = 0.0;
	float spec = 0.0;
	float shadow = 0.0;

	diff = max(dot(vertNormal, lightDir), 0.0);

	
	if(diff > 0.0)
	{
		vec3 reflection = normalize(reflect(-lightDir, vertNormal));
		spec = max(dot(inverse(mat3(viewMatrix)) * eyeDir, reflection), 0.0);
		spec = pow(spec, 30.0);
	}
	
	//shadow calcs
	vec3 normalizedSpaceShadows = lightSpacePos.xyz / lightSpacePos.w;
	normalizedSpaceShadows = normalizedSpaceShadows * 0.5 + 0.5;
	float closestDepth = texture(depthMap, normalizedSpaceShadows.xy).z;
	float currentDepth = normalizedSpaceShadows.z;

	float cosTheta = clamp(dot(lightPosition, vertNormal),0,1);
	float bias = 0.005*tan(acos(cosTheta)); // cosTheta is dot( n,l ), clamped between 0 and 1
	bias = clamp(bias, 0,0.01); 

	vec2 texSize = 1.0 / textureSize(depthMap, 0);
	for(int x = -2; x <= 2; x++)
	{
		for(int y = -2; y <= 2; y++)
		{
			float adjPixValue = texture(depthMap, normalizedSpaceShadows.xy + vec2(x, y) * texSize).r;
			if(currentDepth + bias < adjPixValue)
			{
				shadow += 1.0;
			}
		}
	}
	shadow /= 25.0;


	fragColor = ka + (shadow) * ((diff * kd) + (spec * ks));	
} 

