#pragma once

#include <fstream>

#include "animationframe.h"

class VniAnimationFrame : public AnimationFrame {

public:
	VniAnimationFrame(ifstream& is, int file_version, unsigned int time);
	~VniAnimationFrame();

private:
	void read_planes(ifstream& is, int planeSize);
};