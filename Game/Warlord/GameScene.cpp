#include "stdafx.h"
#include "GameScene.h"
#include "NanoWindow.h"
#include "NanoIO.h"
#include "NanoLog.h"
//=============================================================================
bool GameScene::Init()
{
	m_data.Init();

	const auto wndWidth = window::GetWidth();
	const auto wndHeight = window::GetHeight();

	if (!m_mainScene.Init(wndWidth, wndHeight))
		return false;
	if (!m_composite.Init(wndWidth, wndHeight))
		return false;

	return true;
}
//=============================================================================
void GameScene::Close()
{
	m_mainScene.Close();
	m_composite.Close();
}
//=============================================================================
void GameScene::Bind(Camera* camera)
{
	m_data.camera = camera;
}
//=============================================================================
void GameScene::Bind(GameModel* go)
{
	m_data.Bind(go);
}
//=============================================================================
void GameScene::Draw()
{
	if (!m_data.camera)
	{
		Warning("Not active camera");
		return;
	}
	beginDraw();
	draw();
	endDraw();
}
//=============================================================================
void GameScene::beginDraw()
{
	const auto wndWidth = window::GetWidth();
	const auto wndHeight = window::GetHeight();

	m_mainScene.Resize(wndWidth, wndHeight);
	m_composite.Resize(wndWidth, wndHeight);
}
//=============================================================================
void GameScene::draw()
{
	m_mainScene.Draw(m_data);
	m_composite.Draw(&m_mainScene.GetFBO());
	blittingToScreen(m_composite.GetFBOId(), m_composite.GetWidth(), m_composite.GetHeight());
}
//=============================================================================
void GameScene::endDraw()
{
	m_data.ResetFrame();
}
//=============================================================================
void GameScene::blittingToScreen(GLuint fbo, uint16_t srcWidth, uint16_t srcHeight)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(
		0, 0, srcWidth, srcHeight,
		0, 0, window::GetWidth(), window::GetHeight(),
		GL_COLOR_BUFFER_BIT, GL_NEAREST);
}
//=============================================================================