#pragma once

#include <cstdint>

/// <summary>
/// An animation plane is a 1-bit plane 
/// </summary>
class AnimationPlane {

public:
	/// <summary>
	/// Type of plane
	/// </summary>
	uint8_t marker = 0;

	uint8_t *plane = NULL;
};
