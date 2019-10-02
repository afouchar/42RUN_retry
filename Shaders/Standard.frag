#version 410 core

out vec4 out_Color;

in vec2				vUv;
in vec3				vViewPosition;
in vec3				vNormal;

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

// uniform float		MaterialType;

// uniform vec3					LightPos;
// uniform vec3					La;
// uniform vec3					Ld;
// uniform vec3					Ls;

// uniform vec3					Ka;
// uniform vec3					Kd;
// uniform vec3					Ks;
// uniform float					Ns;
// uniform float					Dissolve;

// uniform float					UVScale;
// uniform float					Bump;
// uniform float					Specularity;


float toLinear(float v)
{
	float gamma = 2.2;
	return pow(v, gamma);
}

vec2 toLinear(vec2 v)
{
	float gamma = 2.2;
	return pow(v, vec2(gamma));
}

vec3 toLinear(vec3 v)
{
	float gamma = 2.2;
	return pow(v, vec3(gamma));
}

vec4 toLinear(vec4 v)
{
	return vec4(toLinear(v.rgb), v.a);
}

float toGamma(float v)
{
	float gamma = 2.2;
	return pow(v, 1.0 / gamma);
}

vec2 toGamma(vec2 v)
{
	float gamma = 2.2;
	return pow(v, vec2(1.0 / gamma));
}

vec3 toGamma(vec3 v)
{
	float gamma = 2.2;
	return pow(v, vec3(1.0 / gamma));
}

vec4 toGamma(vec4 v)
{
	return vec4(toGamma(v.rgb), v.a);
}

vec4 textureLinear(sampler2D uTex, vec2 uv)
{
	return toLinear(texture(uTex, uv));
}

float attenuation(float r, float f, float d)
{
	float denom = d / r + 1.0;
	float attenuation = 1.0 / (denom * denom);
	float t = (attenuation - f) / (1.0 - f);
	return max(t, 0.0);
}

mat3 cotangent(vec3 N, vec3 p, vec2 uv)
{
	// get edge vectors of the pixel triangle
	vec3 dp1 = dFdx(p);
	vec3 dp2 = dFdy(p);
	vec2 duv1 = dFdx(uv);
	vec2 duv2 = dFdy(uv);

	// solve the linear system
	vec3 dp2perp = cross(dp2, N);
	vec3 dp1perp = cross(N, dp1);
	vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
	vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;

	// construct a scale-invariant frame 
	float invmax = 1.0 / sqrt(max(dot(T,T), dot(B,B)));
	return mat3(T * invmax, B * invmax, N);
}

vec3 perturb(vec3 map, vec3 N, vec3 V, vec2 texcoord)
{
	mat3 TBN = cotangent(N, -V, texcoord);
	return normalize(TBN * map);
}

float computeDiffuse(vec3 lightDirection, vec3 viewDirection, vec3 surfaceNormal, float roughness, float albedo)
{

	float LdotV = dot(lightDirection, viewDirection);
	float NdotL = dot(lightDirection, surfaceNormal);
	float NdotV = dot(surfaceNormal, viewDirection);

	float s = LdotV - NdotL * NdotV;
	float t = mix(1.0, max(NdotL, NdotV), step(0.0, s));

	float sigma2 = roughness * roughness;
	float A = 1.0 + sigma2 * (albedo / (sigma2 + 0.13) + 0.5 / (sigma2 + 0.33));
	float B = 0.45 * sigma2 / (sigma2 + 0.09);

	return albedo * max(0.0, NdotL) * (A + B * s / t) / 3.14159265;
}

float phongSpecular(vec3 lightDirection, vec3 viewDirection, vec3 surfaceNormal, float shininess)
{
	vec3 R = -reflect(lightDirection, surfaceNormal);
	return pow(max(0.0, dot(viewDirection, R)), shininess);
}

vec4 CelShading (vec4 color, vec4 light, vec3 nor)
{
	float factor;
	float Intensity = dot(light.xyz , normalize(nor));
	if (Intensity < 0.4)
		factor = 0.4;
	else if (Intensity < 0.8)
		factor = 0.8;
	else if (Intensity < 0.1)
		factor = 0.1;
	else
		factor = 1.0;
	color *= vec4(factor, factor, factor, 1.0);
	return color;
}

void main()
{
	vec4 lightPos = V * vec4(LightPosition, 1.0);
	// if (MaterialType < 1)
	// {
	// 	vec3 L = normalize(lightPos.xyz - vViewPosition);   
	// 	vec3 E = normalize(-vViewPosition); // we are in Eye Coordinates, so EyePos is (0,0,0)  
	// 	vec3 R = normalize(-reflect(L, vNormal));  

	// 	vec4 Iamb = vec4(La, 0.0);

	// 	vec4 Idiff = vec4(Ld * max(dot(vNormal, L), 0.0), 0.0);

	// 	vec4 Ispec = vec4(Ls * pow(max(dot(R, E), 0.0),0.3 * Ns), 0.0);

	// 	vec4 color = vec4(Ka, Dissolve) + Iamb + Idiff + Ispec;
	// 	if (LightShading == 1)
	// 		color = CelShading(color, vec4(L, 1.0), vNormal);
	// 	out_Color = color;
	// }
	// else
	// {
		// vec2 UV_SCALE = vec2(UVScale, UVScale);
		vec3 normal = vNormal;

		vec3 lightVector = -lightPos.xyz + vViewPosition;
		// vec4 color = vec4(Ka, Dissolve);
		vec4 color = vec4(diffuse, 1.0);

		float lightDistance = length(lightVector);
		// float falloff = attenuation(LightRadius, LightFalloff, lightDistance);
		float falloff = attenuation(4.0, 1.0, lightDistance);

		// vec2 uv = vUv * UV_SCALE;
		vec2 uv = vUv;

		vec3 diffuseColor = textureLinear(texture_diffuse1, uv).rgb;
		// vec3 normalMap = textureLinear(texture_normal1, uv).rgb * Bump - 1.0;
		vec3 normalMap = textureLinear(texture_normal1, uv).rgb * bumpScale - 1.0;
		// float specularStrength = textureLinear(texture_specular1, uv).r * Specularity;
		float specularStrength = textureLinear(texture_specular1, uv).r * specular.r;
		normalMap.y *= -1.0;

		vec3 L = normalize(lightVector);              //light direction
		vec3 Vnormalized = normalize(vViewPosition);            //eye direction
		vec3 N = perturb(normalMap, normal, -Vnormalized, vUv); //surface normal

		// float specular = specularStrength * phongSpecular(L, V, N, Ns) * LightSpecScale * falloff;
		float newSpecular = specularStrength * phongSpecular(L, Vnormalized, N, shininess) * falloff;
		// vec3 diffuse = Ld * computeDiffuse(L, V, N, LightRoughness, LightAlbedo) * falloff;
		vec3 newDiffuse = diffuse * computeDiffuse(L, Vnormalized, N, ambient.r, LightColor.r) * falloff;
		// vec3 ambient = La;

		color += vec4(diffuseColor, 0.0) * vec4((newDiffuse + ambient), 0.0) + newSpecular;

		// color = toGamma(color);
		// if (LightShading == 1)
			// out_Color = CelShading(color, vec4(L, 1.0), N);
		// else
			out_Color.rgb = color.rgb;
		vec4 alpha = texture(texture_diffuse1, uv);
		// out_Color.a = max(alpha.a, Dissolve) - abs(alpha.a - Dissolve);
		out_Color.a = max(alpha.a, 1.0) - abs(alpha.a - 1.0);
		// out_Color = vec4(1.0, 0.0, 0.0, 1.0);
	// }
}
