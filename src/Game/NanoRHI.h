#pragma once

struct Vec2;
struct Vec3;
struct Vec4;
struct Quat;
struct Mat3;
struct Mat4;

struct VertexAttribute final
{
	GLuint      index;
	GLenum      type;
	GLint       size;
	const void* offset; //  (void*)offsetof(Vertex, uv)
	bool        normalized{ false };
	bool        perInstance{ false };
};

GLuint CreateShaderProgram(std::string_view vertexShader);
GLuint CreateShaderProgram(std::string_view vertexShader, std::string_view fragmentShader);
GLuint CreateShaderProgram(std::string_view vertexShader, std::string_view geometryShader, std::string_view fragmentShader);

void SpecifyVertexAttributes(GLuint shaderProgram, size_t vertexSize, std::span<const VertexAttribute> attributes);

int GetUniformLocation(GLuint program, std::string_view name);
void SetUniform(GLuint id, float s);
void SetUniform(GLuint id, int s);
void SetUniform(GLuint id, const Vec2& v);
void SetUniform(GLuint id, const Vec3& v);
void SetUniform(GLuint id, const Vec4& v);
void SetUniform(GLuint id, const Quat& v);
void SetUniform(GLuint id, const Mat3& m);
void SetUniform(GLuint id, const Mat4& m);