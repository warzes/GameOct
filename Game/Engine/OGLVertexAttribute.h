#pragma once

#include "OGLEnum.h"

//=============================================================================
// VertexAttribute
//=============================================================================

struct VertexAttribute final
{
	DataType    type{ DataType::Float };
	GLint       count{ 1 };
	const void* offset{ nullptr }; //  EXAMPLE: (void*)offsetof(Vertex, uv)
	bool        normalized{ false };
	bool        perInstance{ false };
};

void SpecifyVertexAttributes(size_t vertexSize, std::span<const VertexAttribute> attributes);