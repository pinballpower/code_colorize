#include "animation.h"

int Animation::num_frames()
{
	return frames.size();
}

int Animation::bit_length()
{
	if (frames.size() > 0) {
		return frames[0]->bit_length;
	}
	else {
		return 0;
	}
}

int Animation::remaining_frames()
{
	return 0;
}

bool Animation::add_planes()
{
	return (switch_mode == ModeFollow) || (switch_mode == ModeColorMask);
}

void Animation::start(SwitchMode mode)
{
}

Animation::Animation(long offset)
{
	this->offset = offset;
}

Animation::Animation()
{
}

