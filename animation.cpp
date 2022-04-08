#include "animation.h"

int Animation::num_frames()
{
	return frames.size();
}

int Animation::bit_length()
{
	if (frames.size() > 0) {
		return frames[0].bit_length;
	}
	else {
		return 0;
	}
}

Animation::Animation()
{
	width = 0;
	height = 0;
	animation_duration = 0;
	offset = 0;
}
