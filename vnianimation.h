#pragma once

#include <fstream>
#include <cstdint>

#include "animation.h"

class VniAnimation : public Animation
{
public:
	VniAnimation(ifstream& is, int file_version);
	void read_palettes_and_colors(ifstream& is);
};
