#pragma once

#if defined(_WIN32)

class Input final
{
	friend class Window;
public:
	void Init();

	void Update();

	// Keyboard
	bool GetKeyDown(size_t vk) const;
	bool GetKeyUp(size_t vk) const;
	bool GetKeyPress(size_t vk) const;

	// Mouse
	bool GetMouseDown(size_t button) const;
	bool GetMouseUp(size_t button) const;
	bool GetMousePress(size_t button) const;
	POINT GetMousePos() const { return m_mousePos; }
	POINT GetMouseDelta() const { return m_mouseDelta; }
	int GetMouseWheel() const { return m_mouseWheel; }

	void SetCursorVisible(bool visible);
	void CaptureMouse(HWND hWnd);
	void ReleaseMouse();
	bool IsMouseCaptured() const { return m_mouseCaptured; }

	// Char
	void ClearCharBuffer();
	const std::vector<wchar_t>& GetCharBuffer() const { return m_charBuffer; }
	void PushChar(wchar_t ch);

	bool IsMouseLeave() const { return m_mouseLeave; }
	void ResetMouseLeave() { m_mouseLeave = false; }
	void SetMouseLeave() { m_mouseLeave = true; }

	bool IsWindowFocused(HWND hwnd) const;

private:
	static constexpr size_t KEY_COUNT = 256u;
	static constexpr size_t MOUSE_BUTTONS = 5u;

	bool  m_keyState[KEY_COUNT] = { false };
	bool  m_keyPrevState[KEY_COUNT] = { false };
	bool  m_mouseState[MOUSE_BUTTONS] = { false };
	bool  m_mousePrevState[MOUSE_BUTTONS] = { false };
	POINT m_mousePos{ 0, 0 };
	POINT m_mousePrevPos{ 0, 0 };
	POINT m_mouseDelta{ 0, 0 };
	int   m_mouseWheel{ 0 };
	bool  m_cursorVisible{ true };
	bool  m_mouseCaptured{ false };

	std::vector<wchar_t> m_charBuffer;

	bool m_mouseLeave = false;
};

#endif // _WIN32