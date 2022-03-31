#pragma once

#include <cstdint>

class AnimationPlane {

public:
	/// <summary>
	/// Type of plane
	/// </summary>
	uint8_t marker = 0;

	uint8_t *plane = NULL;
};
