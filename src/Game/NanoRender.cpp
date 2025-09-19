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
	glEnable(GL_FRAMEBUFFER_SRGB);
}
//=============================================================================
void Render::EndFrame()
{

}
//=============================================================================