#pragma once

#include <string>
#include <vector>

#include "switchmode.h"
#include "animationframe.h"

using namespace std;

enum AnimationEditMode
{
	Replace, Mask, Fixed
};

class Animation {
public:
	string name;

	int num_frames();

	int bit_length();

	/// <summary>
	/// Technically this is the offset in the animations file, but it is also used as a unique ID of the animation
	/// </summary>
	uint32_t offset;

	/// <summary>
	/// the frames of the animation
	/// </summary>
	vector<AnimationFrame> frames;

	/// <summary>
	/// length in milliseconds
	/// </summary>
	unsigned int animation_duration;

	int width;
	int height;

	/// <summary>
	/// defines how the animation is applied
	/// </summary>
	SwitchMode switch_mode;

	vector<uint8_t*> masks;

	Animation();
	Animation(uint32_t offset);
};
