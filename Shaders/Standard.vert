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
// out vec3 vViewPosition;
out vec3 vPosition_worldspace;
out vec3 vNormal_cameraspace;
out vec3 vEyeDirection_cameraspace;
out vec3 vLightDirection_cameraspace;


void main() {

	mat4 MVP = P * V * M;
	gl_Position = MVP * vec4(in_Position, 1.0f);

	// Position of the vertex, in worldspace : M * position
	vPosition_worldspace = (M * vec4(in_Position, 1)).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( V * M * vec4(in_Position, 1)).xyz;
	vEyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPosition_cameraspace = ( V * vec4(LightPosition, 1)).xyz;
	vLightDirection_cameraspace = LightPosition_cameraspace + vEyeDirection_cameraspace;
	
	// Normal of the the vertex, in camera space
	// Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	vNormal_cameraspace = ( V * M * vec4(in_Normal, 0)).xyz;
	
	// UV of the vertex. No special space for this one.
	vUv = in_TexCoords;
}