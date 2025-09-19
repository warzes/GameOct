#pragma once

//=============================================================================
// Resources
//=============================================================================
class Shader final
{
public:
	Shader(GLenum type) : m_id(glCreateShader(type)) {}
	~Shader() { Reset(); }
	Shader(const Shader&) = delete;
	Shader(Shader&& other) noexcept : m_id(std::exchange(other.m_id, 0)) {}

	Shader& operator=(const Shader&) = delete;
	Shader& operator=(Shader&& other) noexcept
	{
		if (this == &other) return *this;
		Reset();
		m_id = std::exchange(other.m_id, 0);
		return *this;
	}

	void Reset() noexcept
	{
		if (m_id != 0)
		{
			glDeleteShader(m_id);
			m_id = 0;
		}
	}

	[[nodiscard]] GLuint id() const noexcept { return m_id; }
	explicit operator GLuint() const noexcept { return m_id; }

	explicit operator bool() const noexcept { return m_id > 0; }
private:
	GLuint m_id{ 0 };
};

class Program final
{
public:
	Program() : m_id(glCreateProgram()) {}
	~Program() { Reset(); }

	Program(const Program&) = delete;
	Program& operator=(const Program&) = delete;

	Program(Program&& other) noexcept : m_id(std::exchange(other.m_id, 0)) {}

	Program& operator=(Program&& other) noexcept
	{
		if (this == &other) return *this;
		Reset();
		m_id = std::exchange(other.m_id, 0);
		return *this;
	}

	void Reset() noexcept
	{
		if (m_id != 0)
		{
			glDeleteProgram(m_id);
			m_id = 0;
		}
	}

	[[nodiscard]] GLuint id() const noexcept { return m_id; }
	explicit operator GLuint() const noexcept { return m_id; }

	explicit operator bool() const noexcept { return m_id > 0; }
private:
	GLuint m_id{ 0 };
};

enum class GLFunc
{
	VertexArray, Buffer,
	Texture, Sampler,
	Renderbuffer, Framebuffer,
	Query
};

template<GLFunc Func>
class GLObject
{
public:
	GLObject() { invokeCreate(); }
	~GLObject() { Reset(); }
	GLObject(const GLObject&) = delete;
	GLObject(GLObject&& other) noexcept : m_id(std::exchange(other.m_id, 0)) {}

	GLObject& operator=(const GLObject&) = delete;
	GLObject& operator=(GLObject&& other) noexcept
	{
		if (this == &other) return *this;
		Reset();
		m_id = std::exchange(other.m_id, 0);
		return *this;
	}

	void Reset() noexcept
	{
		if (m_id != 0)
		{
			invokeDelete();
			m_id = 0;
		}
	}

	[[nodiscard]] GLuint id() const noexcept { return m_id; }
	explicit operator GLuint() const noexcept { return m_id; }
	explicit operator bool() const noexcept { return m_id > 0; }

private:
	constexpr void invokeCreate()
	{
		if constexpr (Func == GLFunc::VertexArray)       glGenVertexArrays(1, &m_id);
		else if constexpr (Func == GLFunc::Buffer)       glGenBuffers(1, &m_id);
		else if constexpr (Func == GLFunc::Framebuffer)  glGenFramebuffers(1, &m_id);
		else if constexpr (Func == GLFunc::Renderbuffer) glGenRenderbuffers(1, &m_id);
		else if constexpr (Func == GLFunc::Texture)      glGenTextures(1, &m_id);
		else if constexpr (Func == GLFunc::Sampler)      glGenSamplers(1, &m_id);
		else if constexpr (Func == GLFunc::Query)        glGenQueries(1, &m_id);
	}

	constexpr void invokeDelete()
	{
		if constexpr (Func == GLFunc::VertexArray)       glDeleteVertexArrays(1, &m_id);
		else if constexpr (Func == GLFunc::Buffer)       glDeleteBuffers(1, &m_id);
		else if constexpr (Func == GLFunc::Framebuffer)  glDeleteFramebuffers(1, &m_id);
		else if constexpr (Func == GLFunc::Renderbuffer) glDeleteRenderbuffers(1, &m_id);
		else if constexpr (Func == GLFunc::Texture)      glDeleteTextures(1, &m_id);
		else if constexpr (Func == GLFunc::Sampler)      glDeleteSamplers(1, &m_id);
		else if constexpr (Func == GLFunc::Query)        glDeleteQueries(1, &m_id);
	}

	GLuint m_id{ 0 };
};

using VertexArray = GLObject<GLFunc::VertexArray>;
using Buffer = GLObject<GLFunc::Buffer>;

using Texture = GLObject<GLFunc::Texture>;
using Sampler = GLObject<GLFunc::Sampler>;

using Renderbuffer = GLObject<GLFunc::Renderbuffer>;
using Framebuffer = GLObject<GLFunc::Framebuffer>;

using Query = GLObject<GLFunc::Query>;


using ShaderRef = std::shared_ptr<Shader>;
using ProgramRef = std::shared_ptr<Program>;

using VertexArrayRef = std::shared_ptr<VertexArray>;
using BufferRef = std::shared_ptr<Buffer>;

using TextureRef = std::shared_ptr<Texture>;
using SamplerRef = std::shared_ptr<Sampler>;

using RenderbufferRef = std::shared_ptr<Renderbuffer>;
using FramebufferRef = std::shared_ptr<Framebuffer>;

using QueryRef = std::shared_ptr<Query>;

//=============================================================================
// Resource Modify
//=============================================================================

namespace gl
{


	ProgramRef InitProgram(std::string_view vertexShader);
	ProgramRef InitProgram(std::string_view vertexShader, std::string_view fragmentShader);
	ProgramRef InitProgram(std::string_view vertexShader, std::string_view geometryShader, std::string_view fragmentShader);
	ProgramRef InitProgram(ShaderRef vertexShader);
	ProgramRef InitProgram(ShaderRef vertexShader, ShaderRef fragmentShader);
	ProgramRef InitProgram(ShaderRef vertexShader, ShaderRef geometryShader, ShaderRef fragmentShader);




} // namespace gl




// Commands