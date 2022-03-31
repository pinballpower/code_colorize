#pragma once

#include <cstdint>
#include <vector>

#include "animation.h"

class AnimationSet
{
public:
	int version;
	vector <Animation*> animations;

	~AnimationSet();

	Animation find(int offset);
};