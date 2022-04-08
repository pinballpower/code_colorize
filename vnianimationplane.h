#pragma once

#include <fstream>
#include <cstdint>

#include "animationplane.h"

class VniAnimationPlane : public AnimationPlane 
{

public:
	VniAnimationPlane(std::ifstream& is, int plane_size, uint8_t marker);
};
