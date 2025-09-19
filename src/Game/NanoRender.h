#pragma once

#include "NanoRHI.h"

//=============================================================================
// Core
//=============================================================================

//=============================================================================
// Cmd
//=============================================================================
namespace cmd
{
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