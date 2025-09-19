#include "stdafx.h"
#include "NanoRender.h"
#include "NanoCore.h"
//=============================================================================
namespace
{
	uint16_t currentViewportX{ 0u };
	uint16_t currentViewportY{ 0u };
	uint16_t currentViewportWidth{ 0u };
	uint16_t currentViewportHeight{ 0u };

	bool     scissorEnabled{ false };
	uint16_t currentScissorX{ 0u };
	uint16_t currentScissorY{ 0u };
	uint16_t currentScissorWidth{ 0u };
	uint16_t currentScissorHeight{ 0u };
}
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
	const GLchar* strings = sourceGLSL.data();
	GLuint shader = glCreateShader(stage);
	if (!shader)
	{
		Error("Failed to create OpenGL shader object for stage: " + std::string(shaderStageToString(stage)));
		return 0u;
	}
	glShaderSource(shader, 1, &strings, nullptr);
	glCompileShader(shader);

	GLint success{0};
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
		glDeleteShader(shader);
		shader = 0u;
	}

	return shader;
}
//=============================================================================
ShaderProgram::ShaderProgram(std::string_view vertexShader)
	: ShaderProgram(vertexShader, "", "")
{
}
//=============================================================================
ShaderProgram::ShaderProgram(std::string_view vertexShader, std::string_view fragmentShader)
	: ShaderProgram(vertexShader, "", fragmentShader)
{

}
//=============================================================================
ShaderProgram::ShaderProgram(std::string_view vertexShader, std::string_view geometryShader, std::string_view fragmentShader)
{
	assert(!vertexShader.empty() && "A graphics pipeline must at least have a vertex shader");

	struct shader final
	{
		shader(GLuint _id) : id(_id) {}
		~shader() { if (id) glDeleteShader(id); }
		GLuint id{ 0 };
	};

	shader vs(compileShaderGLSL(GL_VERTEX_SHADER, vertexShader));
	if (!vs.id) return;

	shader gs{ 0 };
	if (!geometryShader.empty())
	{
		gs.id = compileShaderGLSL(GL_GEOMETRY_SHADER, geometryShader);
		if (!gs.id) return;
	}

	shader fs{ 0 };
	if (!fragmentShader.empty())
	{
		fs.id = compileShaderGLSL(GL_FRAGMENT_SHADER, fragmentShader);
		if (!fs.id) return;
	}

	GLuint program = glCreateProgram();
	if (!program) return;

	glAttachShader(program, vs.id);
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
		return;
	}

	m_id = program;
}
//=============================================================================
ShaderProgram::~ShaderProgram()
{
	if (m_id)
	{
		glDeleteProgram(m_id);
	}
}
//=============================================================================
ShaderProgram::ShaderProgram(ShaderProgram&& old) noexcept
	: m_id(std::exchange(old.m_id, 0))
{
}
//=============================================================================
ShaderProgram& ShaderProgram::operator=(ShaderProgram&& old) noexcept
{
	if (this == &old) return *this;
	m_id = std::exchange(old.m_id, 0);
	return *this;
}
//=============================================================================
void cmd::BindShaderProgram(const ShaderProgram& pipeline)
{
	assert(pipeline.IsValid());

	glUseProgram(pipeline.Handle());
}
//=============================================================================
void cmd::SetViewport(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
	if (currentViewportX != x || currentViewportY != y 
		|| currentViewportWidth != width || currentViewportHeight != height)
	{
		glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLsizei>(width), static_cast<GLsizei>(height));
		currentViewportX      = x;
		currentViewportY      = y;
		currentViewportWidth  = width;
		currentViewportHeight = height;
	}
}
//=============================================================================
void cmd::SetScissor(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
	if (x == y == width == height == 0u)
	{
		glDisable(GL_SCISSOR_TEST);
	}
	else if (currentScissorX != x || currentScissorY != y
		|| currentScissorWidth != width || currentScissorHeight != height)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	}
	currentScissorX      = x;
	currentScissorY      = y;
	currentScissorWidth  = width;
	currentScissorHeight = height;
}
//=============================================================================
bool Render::Init(const RenderCreateInfo & createInfo)
{
	return true;
}
//=============================================================================
void Render::Close()
{

}
//=============================================================================
void Render::BeginFrame()
{
	//glEnable(GL_FRAMEBUFFER_SRGB);
}
//=============================================================================
void Render::EndFrame()
{

}
//=============================================================================