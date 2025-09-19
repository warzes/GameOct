#pragma once

#include "NanoRHI.h"
#include "NanoMath.h"

//=============================================================================
// Core
//=============================================================================

class Camera : public Transformation
{
public:
	glm::mat4 Projection(float aspectratio) const;

	void ZoomIn(float angle = glm::radians(1.0f));
	void ZoomOut(float angle = glm::radians(1.0f));

	float FieldOfView = glm::radians(60.0f);
	float ZNear = 0.1f;
	float ZFar = 1000.0f;
};


namespace Vertex
{
	template<typename precision>
	struct tP
	{
		glm::tvec3<precision> Position;

		bool operator==(const tP& other) { return Position == other.Position; }
	};

	template<typename precision>
	struct tPC
	{
		glm::tvec3<precision> Position;
		glm::tvec4<precision> Color;

		bool operator==(const tPC& other) {
			return
				Position == other.Position &&
				Color == other.Color;
		}
	};

	template<typename precision>
	struct tPN
	{
		glm::tvec3<precision> Position;
		glm::tvec3<precision> Normal;

		bool operator==(const tPN& other) {
			return
				Position == other.Position &&
				Normal == other.Normal;
		}
	};

	template<typename precision>
	struct tPT
	{
		glm::tvec3<precision> Position;
		glm::tvec2<precision> TexCoord;

		bool operator==(const tPT& other) {
			return
				Position == other.Position &&
				TexCoord == other.TexCoord;
		}
	};

	template<typename precision>
	struct tPTN
	{
		glm::tvec3<precision> Position;
		glm::tvec2<precision> TexCoord;
		glm::tvec3<precision> Normal;

		bool operator==(const tPTN& other) {
			return
				Position == other.Position &&
				TexCoord == other.TexCoord &&
				Normal == other.Normal;
		}
	};

	template<typename precision>
	struct tPTNTB
	{
		glm::tvec3<precision> Position;
		glm::tvec2<precision> TexCoord;
		glm::tvec3<precision> Normal;
		glm::tvec3<precision> Tangent;
		glm::tvec3<precision> Bitangent;

		bool operator==(const tPTNTB& other) {
			return
				Position == other.Position &&
				TexCoord == other.TexCoord &&
				Normal == other.Normal &&
				Tangent == other.Tangent &&
				Bitangent == other.Bitangent;
		}
	};

	template<typename precision>
	struct tPTNTBIW
	{
		glm::tvec3<precision> Position;
		glm::tvec2<precision> TexCoord;
		glm::tvec3<precision> Normal;
		glm::tvec3<precision> Tangent;
		glm::tvec3<precision> Bitangent;
		glm::uvec4 NodeIndices;
		glm::tvec4<precision> NodeWeights;

		bool operator==(const tPTNTBIW& other) {
			return
				Position == other.Position &&
				TexCoord == other.TexCoord &&
				Normal == other.Normal &&
				Tangent == other.Tangent &&
				Bitangent == other.Bitangent &&
				NodeIndices == other.NodeIndices &&
				NodeWeights == other.NodeWeights;
		}
	};


	/* type definitions */
	typedef tP<float> P;
	typedef tPC<float> PC;
	typedef tPN<float> PN;
	typedef tPT<float> PT;
	typedef tPTN<float> PTN;
	typedef tPTNTB<float> PTNTB;
	typedef tPTNTBIW<float> PTNTBIW;
}

template<typename T>
struct Mesh
{
	/* to identify the mesh */
	std::string Name = "... not yet specified ...";

	std::vector<T> Vertices;
	std::vector<GLuint> Indices;
};

struct Material
{
	glm::vec3 Kd;		/* diffuse, 0 .. 1 */
	glm::vec3 Ks;		/* specular, 0 .. 1 */
	float Ns = 1.0f;	/* shininess, 1 .. 1000 */

	GLuint map_Kd = 0;	/* diffuse texture */

	/* etc ... */
};

//=============================================================================
// Cmd
//=============================================================================
namespace cmd
{
	void SetViewport(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
	void SetScissor(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
} // namespace cmd

//=============================================================================
// Render
//=============================================================================

struct RenderCreateInfo final
{
};

class Render final
{
public:
	bool Init(const RenderCreateInfo& createInfo);
	void Close();

	void BeginFrame();
	void EndFrame();
};