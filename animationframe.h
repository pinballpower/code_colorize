#pragma once

#include <cstdint>
#include <vector>

#include "animationplane.h"
#include "../dmd/dmdframe.h"

using namespace std;

class AnimationFrame {

public:
	/// <summary>
	/// Duration of the frame
	/// </summary>
	unsigned int delay = 0;

	/// <summary>
	/// Bit length of the frames (and hence number of planes of each frame). Either 2 or 4.
	/// </summary>
	int bit_length = 0;

	vector<AnimationPlane*> planes;

	/// <summary>
	/// Get the frame in 8-bit per pixel data
	/// </summary>
	/// <returns></returns>
	const vector<uint8_t> get_frame_data() const;

	virtual DMDFrame as_dmd_frame(int width, int height);

protected:

	unsigned int hash = 0;

	/// <summary>
	/// Combine planes and mask and store it in combined
	/// </summary>
	/// <param name="len">Number of pixels</param>
	void combine_planes(int len);

	vector<uint8_t> mask;

	/// <summary>
	/// Combined planes and mask. Planes are bits 0-6 (even if there are less planes), mask is bit 7
	/// </summary>
	vector<uint8_t> combined;



};