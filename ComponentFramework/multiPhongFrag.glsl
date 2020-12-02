#version 410
in  vec3 vertNormal;
in  vec3[3] lightDir;
in  vec3 eyeDir;
//flat in int lightNum;
out vec4 fragColor;

void main() { 
	vec4 ks = vec4(0.6, 0.6, 0.6, 1.0);
	vec4 kd = vec4(1.0, 0.0, 0.0, 1.0); /// const means it cannot be changed just like C++
	vec4 ka = vec4(vec3(0.1 * kd), 1.0);
	float diff = 0.0;
	float spec = 0.0;

	fragColor += ka;

	for(int i = 0; i < 3; i++)
	{
		float tempDiff = max(dot(vertNormal, lightDir[i]), 0.0);
	/// Reflection is based incedent which means a vector from the light source
	/// not the direction to the light source
		vec3 reflection = normalize(reflect(-lightDir[i], vertNormal));
		float tempSpec = max(dot(eyeDir, reflection), 0.0);
	
		if(tempDiff > -0.01)
		{
			tempSpec = pow(tempSpec,14.0);
		}

		ks = vec4(cos(i), sin(i), 1.0, 0.0);
		kd = ks * 0.3;
		fragColor += (tempDiff * kd) + (tempSpec * ks);	
	}

} 

