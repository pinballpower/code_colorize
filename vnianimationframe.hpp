#pragma once

#include <fstream>

#include "animationframe.hpp"

class VniAnimationFrame : public AnimationFrame {

public:
	VniAnimationFrame(ifstream& is, int file_version);

private:
	void read_planes(ifstream& is, int planeSize);
};