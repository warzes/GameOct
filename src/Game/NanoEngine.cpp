#include "stdafx.h"
#include "NanoEngine.h"
#include "NanoCore.h"
//=============================================================================
Window window;
Input  input;
Render render;
bool IsExit{ true };
//=============================================================================
bool app::Init(const AppCreateInfo& createInfo)
{
	IsExit = false;
	if (!window.Init(createInfo.window))
		return false;
	input.Init();
	if (!render.Init(createInfo.render))
		return false;

	return true;
}
//=============================================================================
void app::Close()
{
	IsExit = true;
	render.Close();
	window.Close();
}
//=============================================================================
bool app::IsRunning()
{
	return window.IsRunning() && !IsExit;
}
//=============================================================================
void app::BeginFrame()
{
	window.ProcessEvents();
	input.Update();

	render.BeginFrame();
}
//=============================================================================
void app::EndFrame()
{
	render.EndFrame();
	window.SwapBuffers();
}
//=============================================================================
void app::Exit()
{
	IsExit = true;
}
//=============================================================================