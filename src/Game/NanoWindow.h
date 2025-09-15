#pragma once

#if defined(_WIN32)

struct WindowCreateInfo final
{
	uint16_t     width{ 1600 };
	uint16_t     height{ 900 };
	std::wstring title{ L"Game" };
	bool         fullScreen{ false };
};

struct MonitorInfo final
{
	HMONITOR     monitor;
	RECT         rect;
	float        dpi;
	std::wstring name;
};

class Window final
{
public:
	bool Init(const WindowCreateInfo& createInfo);
	void Close();

	bool ProcessEvents();
	void SwapBuffers();

	bool IsRunning() const noexcept { return m_running; }
	HWND GetHWND() const noexcept { return m_hwnd; }
	HDC GetHDC() const noexcept { return m_hDC; }

	// События окна
	bool IsMinimized() const noexcept { return m_minimized; }
	bool IsMaximized() const noexcept { return m_maximized; }
	bool IsResized() const noexcept { return m_resized; }
	void ResetResized() noexcept { m_resized = false; }

	uint16_t GetWidth() const noexcept { return m_width; }
	uint16_t GetHeight() const noexcept { return m_height; }
	float GetWindowAspect() const noexcept { return m_aspect; }
	
	POINT GetWindowPos() const noexcept;
	void SetWindowPos(int x, int y);
	void SetWindowSize(uint16_t width, uint16_t height);	
	void SetTitle(const std::wstring& title);
	void Minimize();
	void Maximize();
	void Restore();
	void SetTopmost(bool topmost);
	bool IsFocused() const noexcept;
	float GetDPI() const noexcept;
	POINT ClientToScreen(const POINT& pt) const noexcept;
	POINT ScreenToClient(const POINT& pt) const noexcept;

	void SetFullscreen(bool fullscreen);
	bool IsFullscreen() const { return m_fullscreen; }

	MonitorInfo GetCurrentMonitorInfo() const;

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool createOpenGLContext();
	void destroyOpenGLContext();

	HWND     m_hwnd{ nullptr };
	HDC      m_hDC{ nullptr };
	HGLRC    m_hGLRC{ nullptr };
	bool     m_running{ true };
	bool     m_minimized{ false };
	bool     m_maximized{ false };
	bool     m_resized{ false };
	uint16_t m_width{ 0 };
	uint16_t m_height{ 0 };
	float    m_aspect{ 1.0f };

	bool     m_fullscreen{ false };
	RECT     m_windowedRect{ 0, 0, 0, 0 };
	DWORD    m_windowedStyle{ 0 };
};

#endif // _WIN32