#pragma once

#include <string>
#include <vector>

#include "switchmode.h"
#include "animationframe.h"
#include "palette.h"
#include "../dmd/color.h"

using namespace std;

enum AnimationEditMode
{
	Replace, Mask, Fixed
};

/// <summary>
/// How to scale, not sure if we'll need this
/// </summary>
enum ScalerMode
{
	Doubler,
	Scale2x,
};

class Animation {
public:
	string name;

	int num_frames();

	int bit_length();

	/// <summary>
	/// offset of the animation in the file
	/// </summary>
	/// 
	/// <remarks>
	/// needed to load the animation
	/// </remarks>
	long offset;

	/// <summary>
	/// the frames of the animation
	/// </summary>
	vector<AnimationFrame*> frames;

	/// <summary>
	/// length in milliseconds
	/// </summary>
	unsigned int animation_duration;

	int width;
	int height;

	ScalerMode scaler_mode;

	/// <summary>
	/// how many frames still need to be played
	/// </summary>
	int remaining_frames();

	/// <summary>
	/// If yes: add to existing picture, otherwise replace
	/// </summary>
	bool add_planes();

	/// <summary>
	/// defines how the animation is applied
	/// </summary>
	SwitchMode switch_mode;

	/// <summary>
	/// shows if the animation is currently playing
	/// </summary>
	bool IsRunning;

	/// <summary>
	/// Current frame's following mask
	/// </summary>
	uint8_t* replace_mask;

	int _lastTick;
	int _timer;

	/// <summary>
	/// Index of the frame currently displaying (or enhancing).
	/// </summary>
	int _frameIndex;

	/// <summary>
	/// Next hash to look for (in col seq mode)
	/// </summary>
	uint32_t crc32;

	/// <summary>
	/// Mask for "Follow" switch mode.
	/// </summary>
	uint8_t* mask;

	vector<uint8_t*> masks;

	Animation();
	Animation(long offset);
	
	/// <summary>
	/// Starts the animation
	/// </summary>
	void start(SwitchMode mode);

};
