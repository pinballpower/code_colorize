#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>

#include <boost/log/trivial.hpp>

#include "../dmd/color.h"

using namespace std;

class Palette {

public:
	uint32_t index;
	int type; //  0: normal, 1: default
	DMDColor* colors;

	Palette();
	Palette(istream& is);
	Palette(DMDColor colors[]);

	bool is_default();
	bool is_persistent();

};