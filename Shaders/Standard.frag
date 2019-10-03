#version 410 core

out vec4 out_Color;

in vec2				vUv;
in vec4				vNormal;
// in vec3				vViewPosition;
in vec3 vPosition_worldspace;
in vec3 vNormal_cameraspace;
in vec3 vEyeDirection_cameraspace;
in vec3 vLightDirection_cameraspace;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 LightPosition;
uniform vec3 LightColor;
uniform float LightIntensity;

uniform vec3 specular;
uniform vec3 diffuse;
uniform vec3 ambient;
uniform vec3 emissive;
uniform vec3 reflective;
uniform vec3 transparent;
uniform float shininess;
uniform float bumpScale;


void main() {

	// Material properties
	vec3 MaterialDiffuseColor = texture(texture_diffuse1, vUv ).rgb;
	vec3 MaterialAmbientColor = ambient * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = specular;

	// Distance to the light
	float distance = length( LightPosition - vPosition_worldspace );

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( vNormal_cameraspace );
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize( vLightDirection_cameraspace );
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0,1 );
	
	// Eye vector (towards the camera)
	vec3 E = normalize(vEyeDirection_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E,R ), 0,1 );
	
	out_Color.rgb = 
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightIntensity * cosTheta / (distance * distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * LightColor * LightIntensity * pow(cosAlpha, 5) / (distance * distance);

	out_Color.a = texture(texture_diffuse1, vUv).a;
}