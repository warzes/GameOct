#include "stdafx.h"
#include "NanoEngine.h"
//=============================================================================
#if defined(_MSC_VER)
#	pragma comment( lib, "3rdparty.lib" )
#endif
//=============================================================================
const char* sceneVertexSource = R"glsl(
#version 330 core

in layout (location = 0) vec3 in_position;
in layout (location = 1) vec4 in_color;

uniform vec3 position_offset = vec3(0, 0, 0);

out vec4 color;

void main()
{
	gl_Position = vec4(position_offset + in_position, 1);
	color = in_color;
}
)glsl";

const GLchar* sceneFragmentSource = R"glsl(
#version 330 core

in vec4 color;

layout (location = 0) out vec4 out_color;

void main()
{
	out_color = color;
}
)glsl";

struct Vertex
{
	Vec3 position;
	Vec4 color;
};
std::vector<Vertex> vertices;
VertexAttribute attributes[] =
{
	{
		.index = 0,
		.type = GL_FLOAT,
		.size = 3,
		.offset = (const void*)offsetof(Vertex, position)
	},
	{
		.index = 1,
		.type = GL_FLOAT,
		.size = 4,
		.offset = (const void*)offsetof(Vertex, color)
	}

};

GLuint program;
GLuint vao;
GLuint vbo;

bool InitGame()
{
	AppCreateInfo appCI{};

	if (!app::Init(appCI))
		return false;

	program = CreateShaderProgram(sceneVertexSource, sceneFragmentSource);
	
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	vertices.push_back(Vertex(Vec3(0, 0, 0), Vec4(1, 0, 0, 1)));
	vertices.push_back(Vertex(Vec3(1, 0, 0), Vec4(0, 1, 0, 1)));
	vertices.push_back(Vertex(Vec3(0, 1, 0), Vec4(0, 0, 1, 1)));

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	SpecifyVertexAttributes(program, sizeof(Vertex), attributes);

	glBindVertexArray(0);

	return true;
}
//=============================================================================
void CloseGame()
{
	glDeleteProgram(program);
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

	glUseProgram(program);

	static float x = 3.0f;  x += 0.001f;
	Vec3 position_offset = Vec3(sinf(x), 0, 0);
	SetUniform(GetUniformLocation(program, "position_offset"), position_offset);

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