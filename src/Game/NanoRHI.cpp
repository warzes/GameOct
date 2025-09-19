#include "stdafx.h"
#include "NanoRHI.h"
#include "NanoCore.h"
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
[[nodiscard]] inline ShaderRef compileShaderGLSL(GLenum stage, std::string_view sourceGLSL)
{
	const GLchar* strings = sourceGLSL.data();
	ShaderRef shader = std::make_shared<Shader>(stage);
	if (!shader)
	{
		Error("Failed to create OpenGL shader object for stage: " + std::string(shaderStageToString(stage)));
		return nullptr;
	}
	glShaderSource(shader->id(), 1, &strings, nullptr);
	glCompileShader(shader->id());

	GLint success{ 0 };
	glGetShaderiv(shader->id(), GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint infoLength{ 0 };
		glGetShaderiv(shader->id(), GL_INFO_LOG_LENGTH, &infoLength);
		std::string infoLog;
		if (infoLength > 1)
		{
			infoLog.resize(static_cast<size_t>(infoLength - 1)); // исключаем \0
			glGetShaderInfoLog(shader->id(), infoLength, nullptr, infoLog.data());
		}
		else
		{
			infoLog = "<no info log>";
		}

		std::string logError = "OPENGL " + shaderStageToString(stage) + ": Shader compilation failed: " + infoLog;
		if (strings != nullptr) logError += ", Source: \n" + printShaderSource(strings);
		Error(logError);
		return nullptr;
	}

	return shader;
}
//=============================================================================
ProgramRef gl::InitProgram(std::string_view vertexShader)
{
	ShaderRef shader = compileShaderGLSL(GL_VERTEX_SHADER, vertexShader);
	return InitProgram(shader);
}
//=============================================================================
ProgramRef gl::InitProgram(std::string_view vertexShaderSource, std::string_view fragmentShaderSource)
{
	ShaderRef vertexShader = compileShaderGLSL(GL_VERTEX_SHADER, vertexShaderSource);
	if (!vertexShader) return nullptr;
	ShaderRef fragmentShader = compileShaderGLSL(GL_FRAGMENT_SHADER, fragmentShaderSource);
	if (!fragmentShader) return nullptr;
	return InitProgram(vertexShader, fragmentShader);
}
//=============================================================================
ProgramRef gl::InitProgram(std::string_view vertexShaderSource, std::string_view geometryShaderSource, std::string_view fragmentShaderSource)
{
	ShaderRef vertexShader = compileShaderGLSL(GL_VERTEX_SHADER, vertexShaderSource);
	if (!vertexShader) return nullptr;
	ShaderRef geometryShader = compileShaderGLSL(GL_GEOMETRY_SHADER, geometryShaderSource);
	if (!geometryShader) return nullptr;
	ShaderRef fragmentShader = compileShaderGLSL(GL_FRAGMENT_SHADER, fragmentShaderSource);
	if (!fragmentShader) return nullptr;
	return InitProgram(vertexShader, geometryShader, fragmentShader);
}
//=============================================================================
ProgramRef gl::InitProgram(ShaderRef vertexShader)
{
	return InitProgram(vertexShader, nullptr, nullptr);
}
//=============================================================================
ProgramRef gl::InitProgram(ShaderRef vertexShader, ShaderRef fragmentShader)
{
	return InitProgram(vertexShader, nullptr, fragmentShader);
}
//=============================================================================
ProgramRef gl::InitProgram(ShaderRef vertexShader, ShaderRef geometryShader, ShaderRef fragmentShader)
{
	assert(vertexShader && "A graphics pipeline must at least have a vertex shader");

	ProgramRef program = std::make_shared<Program>();

	glAttachShader(program->id(), vertexShader->id());
	if (geometryShader) glAttachShader(program->id(), geometryShader->id());
	if (fragmentShader) glAttachShader(program->id(), fragmentShader->id());

	glLinkProgram(program->id());

	GLint success{ 0 };
	glGetProgramiv(program->id(), GL_LINK_STATUS, &success);
	if (!success)
	{
		GLint length = 512;
		glGetProgramiv(program->id(), GL_INFO_LOG_LENGTH, &length);
		std::string infoLog;
		infoLog.resize(static_cast<size_t>(length + 1), '\0');
		glGetProgramInfoLog(program->id(), length, nullptr, infoLog.data());
		glDeleteProgram(program->id());
		Error("Failed to compile graphics pipeline.\n" + infoLog);
		return nullptr;
	}

	return program;
}
//=============================================================================