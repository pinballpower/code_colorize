#include "animationframe.h"


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
