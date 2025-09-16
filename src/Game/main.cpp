#include "stdafx.h"
#include "NanoEngine.h"
//=============================================================================
#if defined(_MSC_VER)
#	pragma comment( lib, "3rdparty.lib" )
#endif
//=============================================================================
// Вершины треугольника
float verts[] = {
-0.5f, -0.5f,
0.5f, -0.5f,
0.0f,  0.5f
};

GLuint vao = 0, vbo = 0;

std::optional<GraphicsPipeline> pipeline;

bool InitGame()
{
	AppCreateInfo appCI{};

	if (!app::Init(appCI))
		return false;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	GraphicsPipelineInfo pipelineCI{};

	pipelineCI.vertexShader = R"(
#version 330 core
layout(location = 0) in vec2 pos;
void main() { gl_Position = vec4(pos, 0.0, 1.0); })";

	pipelineCI.fragmentShader = R"(
#version 330 core
out vec4 color;
void main() { color = vec4(1, 0.7, 0.2, 1); })";
	
	pipeline = GraphicsPipeline(pipelineCI);
	if (!pipeline || !pipeline->IsValid()) return false;

	return true;
}
//=============================================================================
void CloseGame()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	pipeline = {};
	app::Close();
}
//=============================================================================
void FrameGame()
{
	if (input.GetKeyDown(VK_ESCAPE))
	{
		app::Exit();
		return;
	}

	cmd::SetViewport({ 0u, 0u, window.GetWidth(), window.GetHeight() });
	//glViewport(0, 0, window.GetWidth(), window.GetHeight());
	glClearColor(0.1f, 0.2f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	cmd::BindGraphicsPipeline(*pipeline);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
//=============================================================================
int main(
	[[maybe_unused]] int   argc,
	[[maybe_unused]] char* argv[])
{
	if (InitGame())
	{
		while (app::IsRunning())
		{
			app::BeginFrame();

			FrameGame();

			app::EndFrame();
		}
	}
	CloseGame();
}
//=============================================================================