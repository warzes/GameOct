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
GLuint gl::CreateShaderProgram(std::string_view vertexShader)
{
	return CreateShaderProgram(vertexShader, "", "");
}
//=============================================================================
GLuint gl::CreateShaderProgram(std::string_view vertexShader, std::string_view fragmentShader)
{
	return CreateShaderProgram(vertexShader, "", fragmentShader);
}
//=============================================================================
GLuint gl::CreateShaderProgram(std::string_view vertexShader, std::string_view geometryShader, std::string_view fragmentShader)
{
	GLuint program = glCreateProgram();
	GLuint vs{ 0 };
	GLuint gs{ 0 };
	GLuint fs{ 0 };

	if (!vertexShader.empty())
	{
		vs = compileShaderGLSL(GL_VERTEX_SHADER, vertexShader);
		if (!vs) return 0;
	}
	if (!geometryShader.empty())
	{
		gs = compileShaderGLSL(GL_GEOMETRY_SHADER, geometryShader);
		if (!gs)
		{
			if (vs) glDeleteShader(vs);
			return 0;
		}
	}
	if (!fragmentShader.empty())
	{
		fs = compileShaderGLSL(GL_FRAGMENT_SHADER, fragmentShader);
		if (!fs)
		{
			if (vs) glDeleteShader(vs);
			if (gs) glDeleteShader(gs);
			return 0;
		}
	}

	if (vs) glAttachShader(program, vs);
	if (gs) glAttachShader(program, gs);
	if (fs) glAttachShader(program, fs);

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

	if (vs) glDeleteShader(vs);
	if (gs) glDeleteShader(gs);
	if (fs) glDeleteShader(fs);

	return program;
}
//=============================================================================