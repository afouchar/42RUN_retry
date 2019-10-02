#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 biTangent;

out vec2 UV;
out vec3 Position_worldspace;
// out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
// out vec3 LightDirection_tangentspace;
// out vec3 EyeDirection_tangentspace;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 LightPosition;
uniform vec3 LightColor;
uniform float LightIntensity;


void main() {

	mat4 MVP = P * V * M;
	gl_Position = MVP * vec4(position, 1.0f);

	// Position of the vertex, in worldspace : M * position
	Position_worldspace = (M * vec4(position, 1)).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( V * M * vec4(position, 1)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPosition_cameraspace = ( V * vec4(LightPosition,1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
	
	// Normal of the the vertex, in camera space
	// Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	// Normal_cameraspace = ( V * M * vec4(normal, 0)).xyz;
	
	// UV of the vertex. No special space for this one.
	UV = texCoords;

	// model to camera = ModelView
	// mat3 MV = mat3(V * M);
	// vec3 vertexTangent_cameraspace = MV * tangent;
	// vec3 vertexBitangent_cameraspace = MV * biTangent;
	// vec3 vertexNormal_cameraspace = MV * normal;
	
	// mat3 TBN = transpose(mat3(vertexTangent_cameraspace, vertexBitangent_cameraspace, vertexNormal_cameraspace));
	// // You can use dot products instead of building this matrix and transposing it. See References for details.

	// LightDirection_tangentspace = TBN * LightDirection_cameraspace;
	// EyeDirection_tangentspace =  TBN * EyeDirection_cameraspace;
}