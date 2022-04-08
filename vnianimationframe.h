#pragma once

#include <fstream>

#include "animationframe.h"

class VniAnimationFrame : public AnimationFrame {

public:
	VniAnimationFrame(ifstream& is, int file_version, unsigned int time);
	~VniAnimationFrame();

	virtual uint8_t* get_frame_data();

private:
	void read_planes(ifstream& is, int planeSize);
};