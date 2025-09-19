#include "stdafx.h"
#include "NanoEngine.h"
//=============================================================================
#if defined(_MSC_VER)
#	pragma comment( lib, "3rdparty.lib" )
#endif
//=============================================================================
const char* sceneVertexSource = R"glsl(#version 330 core
layout(location = 0) in vec2 pos;
void main() { gl_Position = vec4(pos, 0.0, 1.0); }
)glsl";

const GLchar* sceneFragmentSource = R"glsl(#version 330 core
out vec4 color;
void main() { color = vec4(1, 0.2, 0.7, 1); }
)glsl";

//struct Vertex {
//	Vec3 position;
//	Vec3 normal;
//	Vec2 uv;
//	// всего 3*4 + 3*4 + 2*4 = 32 байта, выравнивание по 4 байтам — OK
//};
//static_assert(std::is_standard_layout_v<Vertex>, "Vertex must be standard layout");
//static_assert(sizeof(Vertex) == 32, "Vertex size is not 32 bytes");

float verts[] = {
-0.5f, -0.5f,
0.5f, -0.5f,
0.0f,  0.5f
};

GLuint vao = 0, vbo = 0;

ProgramRef program;

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

	program = gl::InitProgram(sceneVertexSource, sceneFragmentSource);

	return true;
}
//=============================================================================
void CloseGame()
{
	program.reset();
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

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
		
	cmd::SetViewport( 0u, 0u, window.GetWidth(), window.GetHeight());
	glClearColor(0.1f, 0.2f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	//cmd::BindGraphicsPipeline(*pipeline);
	glUseProgram(program->id());
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