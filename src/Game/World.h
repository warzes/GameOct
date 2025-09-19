#pragma once

#include "Model.h"


struct World
{
	World();

	bool Initialize();
	void AnimateNextFrame(float timestep);

	/* camera */
	Camera camera;

	/* model */
	Model model;
	double animation_time = 0.0;
};
