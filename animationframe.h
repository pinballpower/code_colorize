#pragma once

#include <cstdint>
#include <vector>

#include "animationplane.h"

using namespace std;

class AnimationFrame {

public:
	/// <summary>
	/// When the frame within its frame sequence is played
	/// </summary>
	unsigned int time = 0;

	/// <summary>
	/// Duration of the frame
	/// </summary>
	unsigned int delay = 0;

	/// <summary>
	/// Bit length of the frames (and hence number of planes of each frame). Either 2 or 4.
	/// </summary>
	int bit_length = 0;

	vector<AnimationPlane*> planes;

	uint8_t* mask = NULL;

	/// <summary>
	/// Return the animation planes as raw data
	/// </summary>
	uint8_t* planedata{};

	unsigned int hash = 0;

	AnimationFrame();
	AnimationFrame(unsigned int time);
};