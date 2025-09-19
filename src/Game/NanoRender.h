#pragma once

#include "NanoRHI.h"

//=============================================================================
// Core
//=============================================================================

//=============================================================================
// Vertex Arrays
//=============================================================================


//=============================================================================
// Shader Program
//=============================================================================

class ShaderProgram final
{
public:
	ShaderProgram(std::string_view vertexShader);
	ShaderProgram(std::string_view vertexShader, std::string_view fragmentShader);
	ShaderProgram(std::string_view vertexShader, std::string_view geometryShader, std::string_view fragmentShader);
	~ShaderProgram();

	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram(ShaderProgram&& old) noexcept;
	ShaderProgram& operator=(const ShaderProgram&) = delete;
	ShaderProgram& operator=(ShaderProgram&& old) noexcept;


	[[nodiscard]] bool IsValid() const noexcept { return m_id > 0; }
	[[nodiscard]] auto Handle() const noexcept { return m_id; }

private:
	unsigned m_id{ 0 };
};


//=============================================================================
// Cmd
//=============================================================================
namespace cmd
{
	void BindShaderProgram(const ShaderProgram& pipeline);

	void SetViewport(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
	void SetScissor(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
} // namespace cmd

//=============================================================================
// Render
//=============================================================================

struct RenderCreateInfo final
{
};

class Render final
{
public:
	bool Init(const RenderCreateInfo& createInfo);
	void Close();

	void BeginFrame();
	void EndFrame();
};