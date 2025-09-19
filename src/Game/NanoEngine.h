#pragma once

#include "NanoCore.h"
#include "NanoWindow.h"
#include "NanoInput.h"
#include "NanoRender.h"

extern Window window;
extern Input  input;
extern Render render;

struct AppCreateInfo final
{
	WindowCreateInfo window{};
	RenderCreateInfo render{};
};

namespace app
{
	bool Init(const AppCreateInfo& createInfo);
	void Close();

	bool IsRunning();
	void BeginFrame();
	void EndFrame();

	void Exit();
} // namespace app