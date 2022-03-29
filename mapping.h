#pragma once 

#include <cstdint>
#include <iostream>

#include "switchmode.h"
#include "palette.h"

using namespace std;

class Mapping {

public:
	static const int LENGTH = 11;
	uint32_t checksum;

	SwitchMode mode;

	uint8_t palette_index;

	/// how long is this palette active before it will be switched back to the 
	/// default palette (0: never switch back)
	uint32_t duration;

	/// byte position of the animation in FSQ/VNI file
	uint32_t offset;

	/// Is this an animation
	bool IsAnimation();

	Mapping(istream& is);
};