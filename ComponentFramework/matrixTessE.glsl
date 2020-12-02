#version 410
layout (triangles, equal_spacing, ccw) in;

in vec3 vertPos_ES[];
in vec3 vertNormal_ES[];
in vec2 uvCoords_ES[];

out vec3 vertPos_G;
out vec3 vertNormal_G;
out vec2 uvCoords_G;

uniform float time;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform sampler3D noiseTexture;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
} 

void main()
{
	vertPos_G = interpolate3D(vertPos_ES[0],vertPos_ES[1],vertPos_ES[2]);
    vertNormal_G = interpolate3D(vertNormal_ES[0],vertNormal_ES[1],vertNormal_ES[2]);
    uvCoords_G = interpolate2D(uvCoords_ES[0],uvCoords_ES[1],uvCoords_ES[2]);

    gl_Position = projectionMatrix * viewMatrix * (vec4(vertPos_G, 1.0) + texture(noiseTexture, vertPos_G * 0.1 + time * 0.1) * 2.0);
}
