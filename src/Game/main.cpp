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

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec4 color;

void main()
{
	gl_Position = Projection * View * Model * vec4(in_position, 1);
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
	glm::vec3 position;
	glm::vec4 color;
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

struct Scene
{
	Scene()
	{
		camera.Position = glm::vec3(0, 0, 10);
		camera.Yaw(glm::radians(180.0f));

		triangles.push_back(Transformation(glm::vec3(-2, 0, 0)));
		triangles.push_back(Transformation(glm::vec3(2, 0, 0)));
		triangles.push_back(Transformation(glm::vec3(2, 0, 2)));
		triangles.push_back(Transformation(glm::vec3(-2, 0, 2)));
	}

	void AnimateNextFrame(float timestep)
	{
		for (auto it = triangles.begin(); it != triangles.end(); it++)
			it->Yaw(glm::radians(90.0f * timestep));
	}

	Camera camera;

	std::list<Transformation> triangles;
};

Scene scene;

bool InitGame()
{
	AppCreateInfo appCI{};

	if (!app::Init(appCI))
		return false;

	glEnable(GL_DEPTH_TEST);

	program = CreateShaderProgram(sceneVertexSource, sceneFragmentSource);
	
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	vertices.push_back(Vertex(glm::vec3(0, 0, 0), glm::vec4(1, 0, 0, 1)));
	vertices.push_back(Vertex(glm::vec3(1, 0, 0), glm::vec4(0, 1, 0, 1)));
	vertices.push_back(Vertex(glm::vec3(0, 1, 0), glm::vec4(0, 0, 1, 1)));

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

	float aspectratio = window.GetWindowAspect();

	glm::mat4 View = scene.camera.LookAt();
	glm::mat4 Projection = scene.camera.Projection(aspectratio);
			
	cmd::SetViewport( 0u, 0u, window.GetWidth(), window.GetHeight());
	glClearColor(0.1f, 0.2f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);
	
	SetUniform(GetUniformLocation(program, "View"), View);
	SetUniform(GetUniformLocation(program, "Projection"), Projection);

	glBindVertexArray(vao);

	for (auto it = scene.triangles.begin(); it != scene.triangles.end(); it++)
	{
		glm::mat4 Model = it->Matrix();
		SetUniform(GetUniformLocation(program, "Model"), Model);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
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