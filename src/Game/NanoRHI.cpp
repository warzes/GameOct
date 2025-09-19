#include "stdafx.h"
#include "NanoRHI.h"
#include "NanoCore.h"
#include "NanoMath.h"
//=============================================================================
[[nodiscard]] inline std::string shaderStageToString(GLenum stage)
{
	switch (stage)
	{
	case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
	case GL_GEOMETRY_SHADER: return "GL_GEOMETRY_SHADER";
	case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
	case GL_COMPUTE_SHADER:  return "GL_COMPUTE_SHADER";
	default: std::unreachable();
	}
}
//=============================================================================
[[nodiscard]] inline std::string printShaderSource(const char* text)
{
	if (!text) return "";

	std::ostringstream oss;
	int line = 1;
	oss << "\n(" << std::setw(3) << std::setfill(' ') << line << "): ";

	while (*text)
	{
		if (*text == '\n')
		{
			oss << '\n';
			line++;
			oss << "(" << std::setw(3) << std::setfill(' ') << line << "): ";
		}
		else if (*text != '\r')
		{
			oss << *text;
		}
		text++;
	}
	return oss.str();
}
//=============================================================================
[[nodiscard]] inline GLuint compileShaderGLSL(GLenum stage, std::string_view sourceGLSL)
{
	if (sourceGLSL.empty())
	{
		Error("Failed to create OpenGL shader object for stage: " + std::string(shaderStageToString(stage)) + ". Source code Empty.");
		return { 0 };
	}

	GLuint shader = glCreateShader(stage);
	if (!shader)
	{
		Error("Failed to create OpenGL shader object for stage: " + std::string(shaderStageToString(stage)));
		return { 0 };
	}
	const GLchar* strings = sourceGLSL.data();
	glShaderSource(shader, 1, &strings, nullptr);
	glCompileShader(shader);

	GLint success{ 0 };
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint infoLength{ 0 };
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
		std::string infoLog;
		if (infoLength > 1)
		{
			infoLog.resize(static_cast<size_t>(infoLength - 1)); // исключаем \0
			glGetShaderInfoLog(shader, infoLength, nullptr, infoLog.data());
		}
		else
		{
			infoLog = "<no info log>";
		}

		std::string logError = "OPENGL " + shaderStageToString(stage) + ": Shader compilation failed: " + infoLog;
		if (strings != nullptr) logError += ", Source: \n" + printShaderSource(strings);
		Error(logError);
		return 0;
	}

	return shader;
}
//=============================================================================
GLuint CreateShaderProgram(std::string_view vertexShader)
{
	return CreateShaderProgram(vertexShader, "", "");
}
//=============================================================================
GLuint CreateShaderProgram(std::string_view vertexShader, std::string_view fragmentShader)
{
	return CreateShaderProgram(vertexShader, "", fragmentShader);
}
//=============================================================================
GLuint CreateShaderProgram(std::string_view vertexShader, std::string_view geometryShader, std::string_view fragmentShader)
{
	struct shader final
	{
		~shader() { if (id) glDeleteShader(id); }
		GLuint id{ 0 };
	};

	shader vs;
	if (!vertexShader.empty())
	{
		vs.id = compileShaderGLSL(GL_VERTEX_SHADER, vertexShader);
		if (!vs.id) return 0;
	}
	shader gs;
	if (!geometryShader.empty())
	{
		gs.id = compileShaderGLSL(GL_GEOMETRY_SHADER, geometryShader);
		if (!gs.id) return 0;
	}
	shader fs;
	if (!fragmentShader.empty())
	{
		fs.id = compileShaderGLSL(GL_FRAGMENT_SHADER, fragmentShader);
		if (!fs.id) return 0;
	}
	if (vs.id == 0 && gs.id == 0 && fs.id == 0)
	{
		Error("Shader not valid");
		return 0;
	}

	GLuint program = glCreateProgram();
	if (vs.id) glAttachShader(program, vs.id);
	if (gs.id) glAttachShader(program, gs.id);
	if (fs.id) glAttachShader(program, fs.id);
	glLinkProgram(program);

	GLint success{ 0 };
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLint length = 512;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		std::string infoLog;
		infoLog.resize(static_cast<size_t>(length + 1), '\0');
		glGetProgramInfoLog(program, length, nullptr, infoLog.data());
		glDeleteProgram(program);
		Error("Failed to compile graphics pipeline.\n" + infoLog);
		program = 0;
	}

	return program;
}
//=============================================================================
void SpecifyVertexAttributes(GLuint shaderProgram, size_t vertexSize, std::span<const VertexAttribute> attributes)
{
	assert(shaderProgram > 0);
	assert(vertexSize > 0);

	glUseProgram(shaderProgram);
	for (size_t i = 0; i < attributes.size(); i++)
	{
		const auto& attr = attributes[i];
		glEnableVertexAttribArray(attr.index);
		glVertexAttribPointer(attr.index, attr.size, attr.type, attr.normalized ? GL_TRUE : GL_FALSE, vertexSize, attr.offset);
		glVertexAttribDivisor(attr.index, attr.perInstance ? 1 : 0);
	}
}
//=============================================================================
int GetUniformLocation(GLuint program, std::string_view name)
{
	return glGetUniformLocation(program, name.data());
}
//=============================================================================
void SetUniform(GLuint id, float s)
{
	glUniform1f(id, s);
}
//=============================================================================
void SetUniform(GLuint id, int s)
{
	glUniform1i(id, s);
}
//=============================================================================
void SetUniform(GLuint id, const Vec2& v)
{
	glUniform2fv(id, 1, &v.x);
}
//=============================================================================
void SetUniform(GLuint id, const Vec3& v)
{
	glUniform3fv(id, 1, &v.x);
}
//=============================================================================
void SetUniform(GLuint id, const Vec4& v)
{
	glUniform4fv(id, 1, &v.x);
}
//=============================================================================
void SetUniform(GLuint id, const Quat& v)
{
	glUniform4fv(id, 1, &v.w);
}
//=============================================================================
void SetUniform(GLuint id, const Mat3& m)
{
	glUniformMatrix3fv(id, 1, GL_FALSE, m.value);
}
//=============================================================================
void SetUniform(GLuint id, const Mat4& m)
{
	glUniformMatrix4fv(id, 1, GL_FALSE, m.value);
}
//=============================================================================