#pragma once

#include "Animation.h"

struct Model
{
	Node Root;
	std::vector<Animation> Animations;

	bool Load(const std::string& filename);

	void Animate(double animation_time, unsigned int animation_index);

	void Draw(std::vector<DrawableNode>& drawable_nodes) const;
};
