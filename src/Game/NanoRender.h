#pragma once

#include "NanoRHI.h"
#include "NanoMath.h"

//=============================================================================
// Core
//=============================================================================

class Camera : public Transformation
{
public:
	glm::mat4 Projection(float aspectratio) const;

	void ZoomIn(float angle = glm::radians(1.0f));
	void ZoomOut(float angle = glm::radians(1.0f));

	float FieldOfView = glm::radians(60.0f);
	float ZNear = 0.1f;
	float ZFar = 1000.0f;
};


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