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

	uint8_t* mask = NULL;

	/// <summary>
	/// Combined planes and mask. Planes are bits 0-6 (even if there are less planes), mask is bit 7
	/// </summary>
	uint8_t* combined = NULL;

	unsigned int hash = 0;

	/// <summary>
	/// Get the frame in 8-bit per pixel data
	/// </summary>
	/// <returns></returns>
	virtual uint8_t* get_frame_data();

	virtual DMDFrame as_dmd_frame(int width, int height);

	// loop through frame
	void start_pixel_loop();
	uint8_t get_next_pixel(bool mask=true);

protected:

	/// <summary>
	/// Combine planes and mask and store it in combined
	/// </summary>
	/// <param name="len">Number of pixels</param>
	void combine_planes(int len);

	uint8_t *current_pixel;

};