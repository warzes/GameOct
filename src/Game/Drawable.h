#pragma once

#include "NanoRender.h"

//=============================================================================
// Drawable.h
//=============================================================================

struct Drawable
{
	Material material;
	DrawElementsBaseVertex mesh_drawcall;

	Drawable() = default;
	Drawable(const Material& material, const DrawElementsBaseVertex& mesh_drawcall)
		: material(material), mesh_drawcall(mesh_drawcall)
	{
	}
};


struct DrawableNode : public Drawable
{
	glm::mat4 transformation;

	DrawableNode() = default;
	DrawableNode(const glm::mat4& transformation, const Material& material, const DrawElementsBaseVertex& mesh_drawcall)
		: Drawable(material, mesh_drawcall), transformation(transformation)
	{
	}
};