#version 410

in Data 
{
	vec3 vertNorm;
	vec3 vertDir;
	vec3 eyeDir;
	vec3 vertPos;
	vec3 lightDir;

} dataIn;

out vec4 fragColor;

uniform mat4 viewMatrix;
uniform samplerCube skybox;

void main() { 

	vec4 specLight = vec4(0.8, 0.8, 0.8, 1.0);
	vec4 AmbientColor = vec4(0.1, 0.1, 0.3, 1.0);
	vec4 amb = AmbientColor * 0.3;
	float spec = 0.0;
	float rimShine = 0.0;
	vec3 rotatedNormal = mat3(viewMatrix) * dataIn.vertNorm;

	//Specular light
	if(dot(dataIn.lightDir, dataIn.vertNorm) > 0.0)
	{
		vec3 reflectedLight = normalize(reflect(-dataIn.lightDir, dataIn.vertNorm));
		spec = max(dot(reflectedLight, -dataIn.eyeDir), 0.0);
		spec = pow(spec, 50);
	}

	//rimshine
	rimShine = length(cross(-dataIn.eyeDir, rotatedNormal));
	rimShine = pow(rimShine, 25);

	//Reflection
	vec3 boxReflection = reflect(inverse(mat3(viewMatrix)) * dataIn.eyeDir, dataIn.vertNorm);
	boxReflection.x *= -1;
	vec4 boxReflectColor = texture(skybox, boxReflection);

	//Refraction
	vec3 boxRefraction = refract(inverse(mat3(viewMatrix)) * dataIn.eyeDir, dataIn.vertNorm, 0.66);
	boxRefraction.x *= -1;
	vec4 boxRefractColor = texture(skybox, boxRefraction);

	float t = length(cross(dataIn.eyeDir, rotatedNormal));

	vec4 boxFinalColor = mix(boxRefractColor, boxReflectColor, t * t);

	fragColor += boxFinalColor;

	fragColor += amb + (rimShine * specLight) + (spec * specLight);
	//fragColor = vec4(dataIn.vertNorm, 1.0);
} 

