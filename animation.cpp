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

uint32_t* Animation::get_colored_frame(int index)
{
	int len = width * height;
	uint32_t *result = new uint32_t[len];

	uint8_t *px_data = frames[index]->get_frame_data();

	for (int i = 0; i < len; i++) {
		result[i] = animation_colors[*(px_data+i)].get_color_data();
	}

	return result;
}

Animation::Animation(long offset)
{
	this->offset = offset;
}

Animation::Animation()
{
}

