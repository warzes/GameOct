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
void SetUniform(GLuint id, const glm::vec2& v);
void SetUniform(GLuint id, const glm::vec3& v);
void SetUniform(GLuint id, const glm::vec4& v);
void SetUniform(GLuint id, const glm::quat& v);
void SetUniform(GLuint id, const glm::mat3& m);
void SetUniform(GLuint id, const glm::mat4& m);

//=============================================================================
// DrawCommands.h
//=============================================================================

/* basic drawing */
/**********************************************************************************/
struct DrawArrays
{
	GLenum Mode;
	GLint First;
	GLsizei Count;
};

struct DrawElements
{
	GLenum Mode;
	GLsizei Count;
	GLenum Type;
	const GLvoid* Indices;
};

/* multi-draw */
/**********************************************************************************/
struct MultiDrawArrays
{
	GLenum Mode;
	const GLint* First;
	const GLsizei* Count;
	GLsizei DrawCount;
};

struct MultiDrawElements
{
	GLenum Mode;
	const GLsizei* Count;
	GLenum Type;
	const GLvoid* const* Indices;
	GLsizei DrawCount;
};

/* base index */
/**********************************************************************************/
struct DrawElementsBaseVertex
{
	GLenum Mode;
	GLsizei Count;
	GLenum Type;
	const GLvoid* Indices;
	GLint BaseVertex;
};

/* instancing */
/**********************************************************************************/
struct DrawArraysInstanced
{
	GLenum Mode;
	GLint First;
	GLsizei Count;
	GLsizei InstanceCount;
};

struct DrawElementsInstanced
{
	GLenum Mode;
	GLsizei Count;
	GLenum Type;
	const GLvoid* Indices;
	GLsizei InstanceCount;
};

/* range */
/**********************************************************************************/
struct DrawRangeElements
{
	GLenum Mode;
	GLuint Start;
	GLuint End;
	GLsizei Count;
	GLenum Type;
	const GLvoid* Indices;
};

/* combinations */
/**********************************************************************************/
struct MultiDrawElementsBaseVertex
{
	GLenum Mode;
	const GLsizei* Count;
	GLenum Type;
	const GLvoid* const* Indices;
	GLsizei Drawcount;
	const GLint* BaseVertex;
};

struct DrawRangeElementsBaseVertex
{
	GLenum Mode;
	GLuint Start;
	GLuint End;
	GLsizei Count;
	GLenum Type;
	const GLvoid* Indices;
	GLint BaseVertex;
};

struct DrawElementsInstancedBaseVertex
{
	GLenum Mode;
	GLsizei Count;
	GLenum Type;
	const GLvoid* Indices;
	GLsizei PrimCount;
	GLint BaseVertex;
};

/* render commands */
/**********************************************************************************/
inline void Draw(const DrawArrays& cmd)
{
	glDrawArrays(cmd.Mode, cmd.First, cmd.Count);
}

inline void Draw(const DrawElements& cmd)
{
	glDrawElements(cmd.Mode, cmd.Count, cmd.Type, cmd.Indices);
}

inline void Draw(const MultiDrawArrays& cmd)
{
	glMultiDrawArrays(cmd.Mode, cmd.First, cmd.Count, cmd.DrawCount);
}

inline void Draw(const MultiDrawElements& cmd)
{
	glMultiDrawElements(cmd.Mode, cmd.Count, cmd.Type, cmd.Indices, cmd.DrawCount);
}

inline void Draw(const DrawElementsBaseVertex& cmd)
{
	glDrawElementsBaseVertex(cmd.Mode, cmd.Count, cmd.Type, (void*)cmd.Indices, cmd.BaseVertex);
}

inline void Draw(const DrawArraysInstanced& cmd)
{
	glDrawArraysInstanced(cmd.Mode, cmd.First, cmd.Count, cmd.InstanceCount);
}

inline void Draw(const DrawElementsInstanced& cmd)
{
	glDrawElementsInstanced(cmd.Mode, cmd.Count, cmd.Type, cmd.Indices, cmd.InstanceCount);
}

inline void Draw(const DrawRangeElements& cmd)
{
	glDrawRangeElements(cmd.Mode, cmd.Start, cmd.End, cmd.Count, cmd.Type, cmd.Indices);
}

inline void Draw(const MultiDrawElementsBaseVertex& cmd)
{
	glMultiDrawElementsBaseVertex(cmd.Mode, (GLsizei*)cmd.Count, cmd.Type, (void**)cmd.Indices, cmd.Drawcount, (GLint*)cmd.BaseVertex);
}

inline void Draw(const DrawRangeElementsBaseVertex& cmd)
{
	glDrawRangeElementsBaseVertex(cmd.Mode, cmd.Start, cmd.End, cmd.Count, cmd.Type, (void*)cmd.Indices, cmd.BaseVertex);
}

inline void Draw(const DrawElementsInstancedBaseVertex& cmd)
{
	glDrawElementsInstancedBaseVertex(cmd.Mode, cmd.Count, cmd.Type, cmd.Indices, cmd.PrimCount, cmd.BaseVertex);
}