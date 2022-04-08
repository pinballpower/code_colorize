#pragma once

#include <cstdint>
#include <map>

#include "animation.h"

class AnimationSet
{
public:
	int version;
	map<uint32_t, Animation*> animations;

	Animation* find(int offset);
};