#include <cassert>

#include "animationframe.h"
#include "../dmd/dmdframe.h"


AnimationFrame::AnimationFrame()
{
}

AnimationFrame::AnimationFrame(unsigned int time)
 {
	 this->time = time;
 }

uint8_t* AnimationFrame::get_frame_data()
{
	return nullptr;
}

DMDFrame AnimationFrame::as_dmd_frame(int width, int height)
{
	return DMDFrame(width, height, 8, this->get_frame_data());
}

void AnimationFrame::start_pixel_loop()
{
	current_pixel = combined;
}

uint8_t AnimationFrame::get_next_pixel(bool mask)
{
	uint8_t res = *current_pixel;
	if (!mask) {
		res = res & 0x7f;  // mask is bit 7
	}
	current_pixel++;
	return res;
}

void AnimationFrame::combine_planes(int len)
{
	len = len * 8;

	int bit = 7;
	int offset = 0;

	delete[] combined;
	combined = new uint8_t[len];
	uint8_t* px = combined;

	uint8_t bits = planes.size();
	uint8_t maxval = 0xff >> (8 - bits);

	for (; len; len--) {

		uint8_t pv = 0;
		uint8_t planebit = 0;
		for (AnimationPlane* p: planes) {
			// get n'th bit and set it
			pv |= ((p->plane[offset] >> bit) & 0x01) << planebit;
			planebit++;
		}

		assert(pv <= maxval);
		
		pv |= ((mask[offset] >> bit) & 0x01) << 7;

		// next pixel
		bit--;
		if (bit < 0 ) {
			offset++;
			bit = 7;
		}

		*px = pv;
		px++;
	}
}
