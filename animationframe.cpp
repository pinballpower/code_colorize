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

DMDFrame* AnimationFrame::as_dmd_frame(int width, int height)
{
	return new DMDFrame(width, height, 8, (uint32_t*)this->get_frame_data());
}
