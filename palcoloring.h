#pragma once

#include <cstring>
#include <map>
#include <vector>

#include "palpalette.h"
#include "palmapping.h"

using namespace std;

class PalColoring {
public:
	string filename;
	int version; // 1 = FSQ, 2 = VNI 
	map<uint16_t, PalPalette*> palettes;
	map<uint32_t, PaletteMapping*> mappings;
	vector<uint8_t*> masks;
	int default_palette_index = -1;
	int num_palettes = 0;

	PalColoring();
	PalColoring(string filename);
	Palette* get_palette(uint32_t index);
	PaletteMapping* find_mapping(uint32_t checksum);
};