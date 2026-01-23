#version 330 core

struct SphereLight
{
	vec3 position;
	float radius;
	vec3 startColor;
	vec3 endColor;
	float intensity;  // Яркость света
};

// Параметры тумана
struct Fog
{
	vec3 color;
	float density;
	float gradient;
	float start;
	float end;
};

in VS_OUT {
	vec3 vertColor;
	vec3 fragPos;
	vec3 normal;
	vec2 texCoords;
} fs_in;

const float alphaClippingThreshold = 0.1;

uniform sampler2D diffuseTexture;
uniform bool hasDiffuseTex;

//uniform SphereLight sphereLight[4];
//uniform Fog fog;
uniform vec3 viewPos; // camera pos

layout(location = 0) out vec4 FragColor;

vec3 calculateSphereLight(in SphereLight light, in vec3 fragPos)
{
	float distance = length(fragPos - light.position);
	if (distance > light.radius) return vec3(0.0);

	float t = clamp(distance / light.radius, 0.0, 1.0);
	vec3 color = mix(light.startColor, light.endColor, t);
	float attenuation = 1.0 - t;
	return color * attenuation * light.intensity;
}

float calculateFogFactor(in Fog fog, float distance)
{
	float fogFactor = 0.0;
	if(fog.end != fog.start && fog.density == 0.0)
	{
		// Линейный туман
		fogFactor = (fog.end - distance) / (fog.end - fog.start);
		fogFactor = clamp(fogFactor, 0.0, 1.0);
	}
	else if(fog.density > 0.0)
	{
		// Экспоненциальный квадратичный туман
		fogFactor = exp(-pow(fog.density * distance, 2.0));
		fogFactor = clamp(fogFactor, 0.0, 1.0);
	}
	return fogFactor;
}

void main()
{
	// SphereLight light;
	// light.position = vec3(0.0, 0.0, 0.0);
	// light.radius = 3.0;
	// light.startColor = vec3(1.0, 0.0, 0.0);
	// light.endColor = vec3(0.0, 0.0, 0.2);
	// light.intensity = 5.0;

	Fog fog;
	fog.color = vec3(0.5, 0.5, 0.8);
	fog.start = 50.0;
	fog.end = 100.0;
	fog.density = 0.1;

	vec4 diffuse = vec4(fs_in.vertColor, 1.0);
	if (hasDiffuseTex) 
		diffuse = texture(diffuseTexture, fs_in.texCoords) * diffuse;
	if (diffuse.a < alphaClippingThreshold) discard;

	float distance = length(viewPos - fs_in.fragPos);
	vec3 viewDir = normalize(viewPos - fs_in.fragPos);

	//vec3 lightResult = calculateSphereLight(light, fs_in.fragPos);

	//vec3 finalColor = min(diffuse.rgb + lightResult, vec3(1.0));
	vec3 finalColor = diffuse.rgb;

	// fog
	finalColor = mix(fog.color, finalColor, calculateFogFactor(fog, distance));

	//float rim = 1.0 - dot(fs_in.normal, viewDir);
	//rim = pow(rim, 3.0); // Усиление эффекта
	//finalColor += rim * vec3(0.5, 0.5, 1.0); // Синеватый оттенок

	// TODO: Subsurface scattering
	// Rim


	FragColor = vec4(finalColor, diffuse.a);
}