#pragma once

#include <cstring>
#include <map>
#include <vector>

#include "palette.h"
#include "mapping.h"

using namespace std;

class Coloring {
public:
	string filename;
	int version; // 1 = FSQ, 2 = VNI 
	vector<Palette*> palettes;
	map<uint32_t, Mapping*> mappings;
	uint8_t* masks;
	Palette *default_palette;
	uint8_t default_palette_index;
	int num_palettes;

	Coloring();
	Coloring(string filename);
	Palette* get_palette(uint32_t index);
	Mapping* find_mapping(uint32_t checksum);

};