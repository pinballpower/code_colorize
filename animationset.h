#pragma once

#include <cstdint>
#include <map>

#include "animation.h"

class AnimationSet
{
public:
	int version;
	map<uint32_t, Animation> animations;

	const Animation find(int offset);
};