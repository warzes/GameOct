#include "stdafx.h"
#include "NanoInput.h"
#include "NanoWindow.h"
#include "NanoRender.h"
//=============================================================================
#if defined(_MSC_VER)
#	pragma comment( lib, "3rdparty.lib" )
#endif
//=============================================================================
void ExitApp()
{

}
//=============================================================================
Window window;

Input input;
Render render;

//=============================================================================
bool InitApp()
{
	WindowCreateInfo windowCI{};
	RenderCreateInfo renderCI{};

	if (!window.Init(windowCI))
		return false;
	input.Init();
	if (!render.Init(renderCI))
		return false;

	return true;
}
//=============================================================================
void CloseApp()
{
	render.Close();
	window.Close();
}
//=============================================================================
bool FrameApp()
{
	if (!window.IsRunning())
		return false;

	window.ProcessEvents();
	input.Update();

	if (input.GetKeyDown(VK_ESCAPE))
		return false;;

	glClearColor(0.1f, 0.2f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	window.SwapBuffers();

	return true;
}
//=============================================================================
int main(
	[[maybe_unused]] int   argc,
	[[maybe_unused]] char* argv[])
{
	if (InitApp())
	{
		while (FrameApp()) {}
	}
	CloseApp();
}
//=============================================================================