#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>

#include <boost/log/trivial.hpp>

#include "../dmd/color.h"

using namespace std;

class Palette {

public:
	vector<DMDColor> colors;
};

class PalPalette : public Palette  {

public:
	uint16_t index;
	int type; //  0: normal, 1: default

	PalPalette();
	~PalPalette();
	PalPalette(istream& is);

	bool is_default();
	bool is_persistent();

};