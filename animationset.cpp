#include "animationset.h"

const Animation AnimationSet::find(int offset)
{
	return animations[offset];
}

const map<uint32_t, Animation> AnimationSet::get_animations() const
{
	return animations;
}
