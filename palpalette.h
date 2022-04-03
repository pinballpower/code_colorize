#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>

#include <boost/log/trivial.hpp>

#include "../dmd/color.h"
#include "../dmd/palette.h"

using namespace std;

class PalPalette : public Palette  {

public:
	uint32_t index;
	int type; //  0: normal, 1: default

	PalPalette();
	~PalPalette();
	PalPalette(istream& is);

	bool is_default();
	bool is_persistent();

};