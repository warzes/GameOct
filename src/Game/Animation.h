#pragma once

#include "Node.h"

//=============================================================================
// Animation.h
//=============================================================================
/* describes how 1 model is animated */
struct Animation
{
	struct AnimatedNode
	{
		unsigned int NodeID = 0;
		Transformation Transform;
	};

	/* describes how 1 node of a model is animated */
	struct Channel
	{
		template<typename T>
		struct Key
		{
			double Time = 0.0;
			T Value;
		};

		enum class State
		{
			Default,	/* value from default node transformation */
			Constant,	/* value from nearest key */
			Linear,		/* value from the nearest two keys extrapolated */
			Repeat,		/* value at (t - t1) % |t2 - t1| */
		} PreState = State::Default, PostState = State::Default;

		unsigned int NodeID = 0;
		std::vector<Key<Transformation>> Keys;

		/* calculate 1 node transformation */
		AnimatedNode Animate(double time_in_ticks, double duration, const Key<Transformation>& key1, const Key<Transformation>& key2) const;

		/* find keys */
		bool FindKeys(double time_in_ticks, Key<Transformation>& key1, Key<Transformation>& key2) const;
	};

	std::string Name = "... not yet specified ...";
	double Duration = 1.0;
	double TicksPerSecond = 1.0;
	std::vector<Channel> Channels;

	/* calculate all node transformations */
	std::vector<AnimatedNode> Animate(double time_in_s) const;

	/* show animation data */
	void Print() const;
};