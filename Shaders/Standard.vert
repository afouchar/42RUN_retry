#version 410 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in vec2 in_TexCoords;
layout (location = 3) in vec3 in_Tangent;
layout (location = 4) in vec3 in_BiTangent;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 LightPosition;
uniform vec3 LightColor;
uniform float LightIntensity;

out vec3 vNormal;
out vec2 vUv;
out vec3 vViewPosition;

void main() {

	mat4 modelViewMatrix = V * M;
	vec4 viewModelPosition = modelViewMatrix * vec4(in_Position, 1.0);

	vViewPosition = viewModelPosition.xyz;
	vUv = in_TexCoords;
	gl_Position = P * viewModelPosition;

	mat3 NormalMatrix = transpose(inverse(mat3(modelViewMatrix)));
	vNormal = normalize(NormalMatrix * in_Normal);
}