#include "stdafx.h"
#if defined(_WIN32)
#include "NanoInput.h"
//=============================================================================
Input* gInputInstance{ nullptr };
//=============================================================================
void Input::Init()
{
	GetCursorPos(&m_mousePos);
	m_mousePrevPos = m_mousePos;
	m_mouseWheel = 0;
	m_mouseCaptured = false;
	m_mouseLeave = false;
	// Инициализация массивов состояний
	for (size_t i = 0; i < KEY_COUNT; ++i)
	{
		m_keyState[i] = false;
		m_keyPrevState[i] = false;
	}
	for (size_t i = 0; i < MOUSE_BUTTONS; ++i)
	{
		m_mouseState[i] = false;
		m_mousePrevState[i] = false;
	}
	m_charBuffer.clear();
	gInputInstance = this;
}
//=============================================================================
void Input::Update()
{
	// Сохраняем предыдущее состояние
	for (size_t i = 0; i < KEY_COUNT; ++i)
		m_keyPrevState[i] = m_keyState[i];
	for (size_t i = 0; i < MOUSE_BUTTONS; ++i)
		m_mousePrevState[i] = m_mouseState[i];

	// Клавиши
	for (size_t vk = 0; vk < KEY_COUNT; ++vk)
		m_keyState[vk] = (GetAsyncKeyState(static_cast<int>(vk)) & 0x8000) != 0;

	// Мышь (0 - левая, 1 - правая, 2 - средняя, 3 - X1, 4 - X2)
	const int mouseVK[MOUSE_BUTTONS] = { VK_LBUTTON, VK_RBUTTON, VK_MBUTTON, VK_XBUTTON1, VK_XBUTTON2 };
	for (size_t btn = 0; btn < MOUSE_BUTTONS; ++btn)
		m_mouseState[btn] = (GetAsyncKeyState(mouseVK[btn]) & 0x8000) != 0;

	m_mousePrevPos = m_mousePos;
	GetCursorPos(&m_mousePos);
	m_mouseDelta.x = m_mousePos.x - m_mousePrevPos.x;
	m_mouseDelta.y = m_mousePos.y - m_mousePrevPos.y;

	// Сброс колеса мыши (ожидается, что оно будет обновляться через обработчик сообщений)
	m_mouseWheel = 0;
	// m_mouseLeave сбрасывается вручную через ResetMouseLeave()
}
//=============================================================================
void Input::ClearCharBuffer()
{
	m_charBuffer.clear();
}
//=============================================================================
void Input::PushChar(wchar_t ch)
{
	m_charBuffer.push_back(ch);
}
//=============================================================================
bool Input::GetKeyDown(size_t vk) const
{
	if (vk >= KEY_COUNT) return false;
	return m_keyState[vk] && !m_keyPrevState[vk];
}
//=============================================================================
bool Input::GetKeyUp(size_t vk) const
{
	if (vk >= KEY_COUNT) return false;
	return !m_keyState[vk] && m_keyPrevState[vk];
}
//=============================================================================
bool Input::GetKeyPress(size_t vk) const
{
	if (vk >= KEY_COUNT) return false;
	return m_keyState[vk];
}
//=============================================================================
bool Input::GetMouseDown(size_t button) const
{
	if (button >= MOUSE_BUTTONS) return false;
	return m_mouseState[button] && !m_mousePrevState[button];
}
//=============================================================================
bool Input::GetMouseUp(size_t button) const
{
	if (button >= MOUSE_BUTTONS) return false;
	return !m_mouseState[button] && m_mousePrevState[button];
}
//=============================================================================
bool Input::GetMousePress(size_t button) const
{
	if (button >= MOUSE_BUTTONS) return false;
	return m_mouseState[button];
}
//=============================================================================
void Input::SetCursorVisible(bool visible)
{
	if (m_cursorVisible != visible) {
		ShowCursor(visible);
		m_cursorVisible = visible;
	}
}
//=============================================================================
void Input::CaptureMouse(HWND hWnd)
{
	if (!m_mouseCaptured)
	{
		SetCapture(hWnd);
		m_mouseCaptured = true;
	}
}
//=============================================================================
void Input::ReleaseMouse()
{
	if (m_mouseCaptured)
	{
		ReleaseCapture();
		m_mouseCaptured = false;
	}
}
//=============================================================================
bool Input::IsWindowFocused(HWND hwnd) const
{
	return GetForegroundWindow() == hwnd;
}
//=============================================================================
#endif // _WIN32