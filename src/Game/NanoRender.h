#pragma once

//=============================================================================
// Core
//=============================================================================

struct Rect2D;

//=============================================================================
// Graphics Pipeline
//=============================================================================
struct GraphicsPipelineInfo final
{
	std::string vertexShader{};
	std::string geometryShader{};
	std::string fragmentShader{};
};

struct GraphicsPipeline final
{
	GraphicsPipeline(const GraphicsPipelineInfo& info);
	~GraphicsPipeline();
	GraphicsPipeline(const GraphicsPipeline&) = delete;
	GraphicsPipeline(GraphicsPipeline&& old) noexcept;
	GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;
	GraphicsPipeline& operator=(GraphicsPipeline&& old) noexcept;

	[[nodiscard]] bool IsValid() const { return m_id > 0; }
	[[nodiscard]] auto Handle() const { return m_id; }

private:
	unsigned m_id{ 0 };
};

//=============================================================================
// Compute Pipeline
//=============================================================================

struct ComputePipelineInfo final
{
	std::string shader{};
};

//=============================================================================
// Cmd
//=============================================================================
namespace cmd
{
	void BindGraphicsPipeline(const GraphicsPipeline& pipeline);

	void SetViewport(const Rect2D& viewport);
	void SetScissor(const Rect2D& scissor);
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