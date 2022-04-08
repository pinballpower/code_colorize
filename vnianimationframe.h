#pragma once

#include <fstream>

#include "animationframe.h"

class VniAnimationFrame : public AnimationFrame {

public:
	VniAnimationFrame(ifstream& is, int file_version);

private:
	void read_planes(ifstream& is, int planeSize);
};