#include "stdafx.h"
#if defined(_WIN32)
#include "NanoWindow.h"
#include "NanoInput.h"
#include "NanoCore.h"
//=============================================================================
#if defined(_MSC_VER)
#	pragma comment(lib, "opengl32.lib")
#endif
//=============================================================================
typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList);
#define WGL_CONTEXT_DEBUG_BIT_ARB         0x00000001
#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001
//=============================================================================
Window* gWindowInstance{ nullptr };
extern Input* gInputInstance;
//=============================================================================
bool Window::Init(const WindowCreateInfo& createInfo)
{
	WNDCLASS wc = {};
	wc.lpfnWndProc = Window::WndProc;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpszClassName = L"NanoWindowClass";
	wc.style = CS_OWNDC;
	RegisterClass(&wc);

	RECT rect = { 0, 0, createInfo.width, createInfo.height };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	m_hwnd = CreateWindow(
		wc.lpszClassName, createInfo.title.c_str(),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr, wc.hInstance, nullptr);

	if (!m_hwnd)
	{
		Fatal("Failed to create window");
		return false;
	}

	m_hDC = GetDC(m_hwnd);
	gWindowInstance = this;
	if (!createOpenGLContext())
		return false;

	return true;
}
//=============================================================================
void Window::Close()
{
	destroyOpenGLContext();
	if (m_hwnd && m_hDC) ReleaseDC(m_hwnd, m_hDC);
	if (m_hwnd) DestroyWindow(m_hwnd);
	UnregisterClass(L"NanoWindowClass", GetModuleHandle(nullptr));
}
//=============================================================================
bool Window::ProcessEvents()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			m_running = false;
			return false;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return m_running;
}
//=============================================================================
void Window::SwapBuffers()
{
	::SwapBuffers(m_hDC);
}
//=============================================================================
POINT Window::GetWindowPos() const noexcept 
{
	RECT rect;
	GetWindowRect(m_hwnd, &rect);
	POINT pt = { rect.left, rect.top };
	return pt;
}
//=============================================================================
void Window::SetWindowPos(int x, int y)
{
	::SetWindowPos(m_hwnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}
//=============================================================================
void Window::SetWindowSize(uint16_t width, uint16_t height)
{
	::SetWindowPos(m_hwnd, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
	m_width = width;
	m_height = height;
}
//=============================================================================
void Window::SetTitle(const std::wstring& title)
{
	SetWindowTextW(m_hwnd, title.c_str());
}
//=============================================================================
void Window::Minimize()
{
	ShowWindow(m_hwnd, SW_MINIMIZE);
}
//=============================================================================
void Window::Maximize()
{
	ShowWindow(m_hwnd, SW_MAXIMIZE);
}
//=============================================================================
void Window::Restore()
{
	ShowWindow(m_hwnd, SW_RESTORE);
}
//=============================================================================
void Window::SetTopmost(bool topmost)
{
	::SetWindowPos(m_hwnd, topmost ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}
//=============================================================================
bool Window::IsFocused() const noexcept
{
	return GetForegroundWindow() == m_hwnd;
}
//=============================================================================
float Window::GetDPI() const noexcept
{
	HDC hdc = GetDC(m_hwnd);
	float dpi = static_cast<float>(GetDeviceCaps(hdc, LOGPIXELSX));
	ReleaseDC(m_hwnd, hdc);
	return dpi;
}
//=============================================================================
POINT Window::ClientToScreen(const POINT& pt) const noexcept
{
	POINT out = pt;
	::ClientToScreen(m_hwnd, &out);
	return out;
}
//=============================================================================
POINT Window::ScreenToClient(const POINT& pt) const noexcept
{
	POINT out = pt;
	::ScreenToClient(m_hwnd, &out);
	return out;
}
//=============================================================================
void Window::SetFullscreen(bool fullscreen)
{
	if (fullscreen == m_fullscreen) return;

	if (fullscreen)
	{
		// Сохраняем параметры оконного режима
		m_windowedStyle = GetWindowLong(m_hwnd, GWL_STYLE);
		GetWindowRect(m_hwnd, &m_windowedRect);

		HMONITOR hMonitor = MonitorFromWindow(m_hwnd, MONITOR_DEFAULTTONEAREST);
		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(hMonitor, &mi);

		SetWindowLong(m_hwnd, GWL_STYLE, m_windowedStyle & ~WS_OVERLAPPEDWINDOW);
		::SetWindowPos(m_hwnd, HWND_TOP,
			mi.rcMonitor.left, mi.rcMonitor.top,
			mi.rcMonitor.right - mi.rcMonitor.left,
			mi.rcMonitor.bottom - mi.rcMonitor.top,
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		m_fullscreen = true;
	}
	else
	{
		// Восстанавливаем параметры оконного режима
		SetWindowLong(m_hwnd, GWL_STYLE, m_windowedStyle);
		::SetWindowPos(m_hwnd, nullptr,
			m_windowedRect.left, m_windowedRect.top,
			m_windowedRect.right - m_windowedRect.left,
			m_windowedRect.bottom - m_windowedRect.top,
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		m_fullscreen = false;
	}
}
//=============================================================================
MonitorInfo Window::GetCurrentMonitorInfo() const
{
	MonitorInfo info = {};
	info.monitor = MonitorFromWindow(m_hwnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFOEXW mi = { {sizeof(mi)} };
	if (GetMonitorInfoW(info.monitor, &mi))
	{
		info.rect = mi.rcMonitor;
		info.name = mi.szDevice;
		HDC hdc = CreateDCW(mi.szDevice, mi.szDevice, nullptr, nullptr);
		info.dpi = hdc ? static_cast<float>(GetDeviceCaps(hdc, LOGPIXELSX)) : 96.0f;
		if (hdc) DeleteDC(hdc);
	}
	return info;
}
//=============================================================================
LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (gWindowInstance)
	{
		switch (msg)
		{
		case WM_SIZE:
			gWindowInstance->m_resized = true;
			if (wParam == SIZE_MINIMIZED)
				gWindowInstance->m_minimized = true;
			else if (wParam == SIZE_MAXIMIZED)
				gWindowInstance->m_maximized = true;
			else
				gWindowInstance->m_minimized = gWindowInstance->m_maximized = false;
			break;
		case WM_CLOSE:
			gWindowInstance->m_running = false;
			PostQuitMessage(0);
			return 0;
			// --- Текстовый ввод ---
		case WM_CHAR:
		case WM_UNICHAR:
			if (gInputInstance && wParam != UNICODE_NOCHAR)
				gInputInstance->PushChar(static_cast<wchar_t>(wParam));
			break;
			// --- Отслеживание выхода мыши ---
		case WM_MOUSELEAVE:
			if (gInputInstance)
				gInputInstance->SetMouseLeave();
			break;
			// --- Отслеживание колеса мыши ---
		case WM_MOUSEWHEEL:
			if (gInputInstance)
				gInputInstance->m_mouseWheel = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
			break;
			// --- Отслеживание входа мыши (для запуска трекинга выхода) ---
		case WM_MOUSEMOVE:
		{
			static bool tracking = false;
			if (!tracking) {
				TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE, hWnd, 0 };
				TrackMouseEvent(&tme);
				tracking = true;
			}
		}
		break;
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//=============================================================================
bool Window::createOpenGLContext()
{
	PIXELFORMATDESCRIPTOR pfd = {};
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pf = ChoosePixelFormat(m_hDC, &pfd);
	SetPixelFormat(m_hDC, pf, &pfd);

	HGLRC tempContext = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, tempContext);

	// Получаем функцию создания контекста 3.3
	auto wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	if (!wglCreateContextAttribsARB)
	{
		Fatal("wglCreateContextAttribsARB not supported");
		return false;
	}

	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	m_hGLRC = wglCreateContextAttribsARB(m_hDC, 0, attribs);
	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(tempContext);
	wglMakeCurrent(m_hDC, m_hGLRC);

	if (gladLoaderLoadGL() == 0)
	{
		Fatal("gladLoaderLoadGL failed");
		return false;
	}

	return true;
}
//=============================================================================
void Window::destroyOpenGLContext()
{
	gladLoaderUnloadGL();
	if (m_hGLRC)
	{
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(m_hGLRC);
		m_hGLRC = nullptr;
	}
}
//=============================================================================
#endif // _WIN32