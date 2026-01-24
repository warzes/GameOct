#include "stdafx.h"
#include "OGLVertexAttribute.h"
//=============================================================================
void SpecifyVertexAttributes(size_t vertexSize, std::span<const VertexAttribute> attributes)
{
	assert(vertexSize > 0);
	assert(attributes.size() > 0);

	for (size_t i = 0; i < attributes.size(); i++)
	{
		const auto& attr = attributes[i];
		const GLuint index = static_cast<GLuint>(i);

		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, attr.count, EnumToValue(attr.type), attr.normalized ? GL_TRUE : GL_FALSE, static_cast<GLsizei>(vertexSize), attr.offset);
		glVertexAttribDivisor(index, attr.perInstance ? 1 : 0);
	}
}
//=============================================================================