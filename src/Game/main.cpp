#include "stdafx.h"
#include "NanoInput.h"
#include "NanoWindow.h"
//=============================================================================
#if defined(_MSC_VER)
#	pragma comment( lib, "3rdparty.lib" )
#endif
//=============================================================================
void ExitApp()
{

}
//=============================================================================
int main(
	[[maybe_unused]] int   argc,
	[[maybe_unused]] char* argv[])
{
	Window window;
	Input input;

	if (window.Init({}))
	{
		input.Init();

		while (window.IsRunning())
		{
			window.ProcessEvents();
			input.Update();

			if (input.GetKeyDown(VK_ESCAPE))
				break;

			glClearColor(0.1f, 0.2f, 0.7f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			window.SwapBuffers();
		}
	}
	window.Close();
}
//=============================================================================