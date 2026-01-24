#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec2 vertexTexCoord;
layout(location = 4) in vec3 vertexTangent;
layout(location = 5) in vec3 vertexBitangent;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out VS_OUT {
	vec3 vertColor;
	vec3 fragPos;
	vec3 normal;
	vec2 texCoords;
} vs_out;

void main()
{
	vs_out.vertColor = vertexColor;
	vs_out.fragPos = vec3(modelMatrix * vec4(vertexPosition, 1.0));
	vs_out.texCoords = vertexTexCoord;
	vs_out.normal = normalize(mat3(transpose(inverse(modelMatrix))) * vertexNormal);
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0f);
}