#include "stdafx.h"
#include "Animation.h"

//=============================================================================
// Animation.h
//=============================================================================
std::vector<Animation::AnimatedNode> Animation::Animate(double time_in_s) const
{
	/* convert seconds to ticks */
	double time_in_ticks = time_in_s * TicksPerSecond;

	/* for each channel that animates 1 node ... */
	std::vector<AnimatedNode> animated_nodes;
	for (unsigned int c = 0; c < Channels.size(); c++)
	{
		Channel::Key<Transformation> key1, key2;
		if (!Channels[c].FindKeys(time_in_ticks, key1, key2))
		{
			//cout << "ERROR: cant find keys in channel " << c << " at time: " << time_in_ticks << " ticks" << endl;
			continue;
		}

		animated_nodes.push_back(Channels[c].Animate(time_in_ticks, Duration, key1, key2));
	}

	return animated_nodes;
}


Animation::AnimatedNode Animation::Channel::Animate(double time_in_ticks, double duration, const Key<Transformation>& key1, const Key<Transformation>& key2) const
{
	/* assert positive duration */
	if (duration < 1.0)
		duration = 1.0;

	/* assert positive animation time */
	if (time_in_ticks < 0.0)
		time_in_ticks = 0.0;

	/* pre-state */
	if (time_in_ticks < key1.Time)
	{
		if (PreState == State::Constant)
		{
			/* nearest key */
			double t_t1 = abs(time_in_ticks - key1.Time);
			double t_t2 = abs(time_in_ticks - key2.Time);
			if (t_t1 < t_t1)
				return { NodeID, key1.Value };
			return { NodeID, key2.Value };
		}
		if (PreState == State::Linear)
		{
			/* linear extrapolation */
			/* TODO: _______ implement extrapolation for Transformation */
			return { NodeID,{} };
		}
		if (PreState == State::Repeat)
		{
			/* repeat */
			double time_in_ticks_fixed = time_in_ticks;
			if (time_in_ticks_fixed > duration)
			{
				unsigned int k = time_in_ticks_fixed / duration;
				time_in_ticks_fixed -= k * time_in_ticks_fixed / duration;
			}
			double weight = (time_in_ticks_fixed - key1.Time) / (key2.Time - key1.Time);
			return { NodeID, Transformation::Interpolate(key1.Value, key2.Value, weight) };
		}

		return { NodeID,{} };
	}
	/* post-state */
	else if (time_in_ticks > key2.Time)
	{
		if (PostState == State::Constant)
		{
			/* nearest key */
			double t_t1 = abs(time_in_ticks - key1.Time);
			double t_t2 = abs(time_in_ticks - key2.Time);
			if (t_t1 < t_t1)
				return { NodeID, key1.Value };
			return { NodeID, key2.Value };
		}
		if (PostState == State::Linear)
		{
			/* linear extrapolation */
			/* TODO: _______ implement extrapolation for Transformation */
			return { NodeID,{} };
		}
		if (PostState == State::Repeat)
		{
			/* repeat */
			double time_in_ticks_fixed = time_in_ticks;
			if (time_in_ticks_fixed > duration)
			{
				unsigned int k = time_in_ticks_fixed / duration;
				time_in_ticks_fixed -= k * time_in_ticks_fixed / duration;
			}
			double weight = (time_in_ticks_fixed - key1.Time) / (key2.Time - key1.Time);
			return { NodeID, Transformation::Interpolate(key1.Value, key2.Value, weight) };
		}

		return { NodeID,{} };
	}

	double weight = (time_in_ticks - key1.Time) / (key2.Time - key1.Time);
	return { NodeID, Transformation::Interpolate(key1.Value, key2.Value, weight) };
}


bool Animation::Channel::FindKeys(double time_in_ticks, Key<Transformation>& key1, Key<Transformation>& key2) const
{
	if (Keys.size() < 2)
		return false;

	/* find nearest */
	int k_nearest = -1;
	double time_nearest = 0;

	for (unsigned int k = 0; k < Keys.size(); k++)
	{
		double time_span = abs(time_in_ticks - Keys[k].Time);
		if (k_nearest == -1)
		{
			k_nearest = k;
			time_nearest = time_span;
		}
		else if (time_span < time_nearest)
		{
			time_nearest = time_span;
			k_nearest = k;
		}
	}

	/* find second nearest */
	int k_second_nearest = -1;
	double time_second_nearest = 0;

	for (unsigned int k = 0; k < Keys.size(); k++)
	{
		if (k == k_nearest)
			continue;
		double time_span = abs(time_in_ticks - Keys[k].Time);
		if (k_second_nearest == -1)
		{
			k_second_nearest = k;
			time_second_nearest = time_span;
		}
		else if (time_span < time_second_nearest)
		{
			time_second_nearest = time_span;
			k_second_nearest = k;
		}
	}

	/* return keys */
	if (Keys[k_nearest].Time > Keys[k_second_nearest].Time)
	{
		key1 = Keys[k_second_nearest];
		key2 = Keys[k_nearest];
	}
	else
	{
		key1 = Keys[k_nearest];
		key2 = Keys[k_second_nearest];
	}

	return true;
}


void Animation::Print() const
{
	//cout << "animation name: " << Name << endl;
	//cout << "\tduration (in ticks): " << Duration << endl;
	//cout << "\tticks per second: " << TicksPerSecond << endl;
	//cout << "\tnumber of channels: " << Channels.size() << endl;

	for (unsigned int c = 0; c < Channels.size(); c++)
	{
		/*cout
			<< "\t\tchannel " << c << ": "
			<< " node ID = " << Channels[c].NodeID
			<< " number of keys: " << Channels[c].Keys.size()
			<< " pre-state: "
			<< (Channels[c].PreState == Channel::State::Default ? "Default" :
				(Channels[c].PreState == Channel::State::Constant ? "Constant" :
					(Channels[c].PreState == Channel::State::Linear ? "Linear" : "Repeat")))
			<< " post-state: "
			<< (Channels[c].PostState == Channel::State::Default ? "Default" :
				(Channels[c].PostState == Channel::State::Constant ? "Constant" :
					(Channels[c].PostState == Channel::State::Linear ? "Linear" : "Repeat")))
			<< endl;*/

		for (unsigned int k = 0; k < Channels[c].Keys.size(); k++)
		{
			/*cout
				<< "\t\t\tkey " << k << ": " << endl
				<< "\t\t\t\ttime = " << Channels[c].Keys[k].Time << endl
				<< "\t\t\t\tposition = " << Channels[c].Keys[k].Value.Position.x << " " << Channels[c].Keys[k].Value.Position.y << " " << Channels[c].Keys[k].Value.Position.z << endl
				<< "\t\t\t\trotation = " << Channels[c].Keys[k].Value.Rotation.w << " " << Channels[c].Keys[k].Value.Rotation.x << " " << Channels[c].Keys[k].Value.Rotation.y << " " << Channels[c].Keys[k].Value.Rotation.z << endl
				<< endl;*/
		}
	}
}