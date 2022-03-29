#include <iostream>
#include <fstream>
#include <cstdint>
#include <filesystem>

#include <boost/log/trivial.hpp>

#include "coloring.h"
#include "mapping.h"
#include "streamhelper.h"

using namespace std;

/// <summary>
/// Default constructor, does nothing. Not to be used.
/// </summary>
Coloring::Coloring()
{
}

/// <summary>
/// Initialize from a .pal color palette file
/// </summary>
/// <param name="filename">File to read</param>
Coloring::Coloring(string filename)
{
	ifstream is;

	if (!(filesystem::exists(filename))) {
		BOOST_LOG_TRIVIAL(error) << "[coloring] file " << filename << " does not exist";
		return;
	};

	auto file_len = std::filesystem::file_size(filename);
	BOOST_LOG_TRIVIAL(trace) << "[coloring] file " << filename << " length " << file_len << " bytes";
	is.open(filename, ios::binary);

	masks = NULL;
	this->filename = filename;
	version = read_u8(is);
	BOOST_LOG_TRIVIAL(trace) << "[coloring] offset " << is.tellg() << " read version as " << version;

	num_palettes = read_u16_be(is);
	BOOST_LOG_TRIVIAL(trace) << "[coloring] offset " << is.tellg() << " read number of palettes as " << num_palettes;

	for (int i = 0; i < num_palettes; i++) {
		Palette *p = new Palette(is);
		palettes.push_back(p);
		if ((default_palette == NULL) && (p->is_default())) {
			default_palette = p;
			default_palette_index = (uint8_t)i;
		}
	}

	if ((default_palette == NULL) && (palettes.size() > 0)) {
		default_palette = palettes[0];
		BOOST_LOG_TRIVIAL(trace) << "[coloring] no default palette defined, using first ";
	}

	int avail = file_len - is.tellg();
	if (avail) {
		is.close();
		return;
	}

	int num_mappings = read_u16_be(is);
	BOOST_LOG_TRIVIAL(trace) << "[coloring] offset " << is.tellg() << " read number of mappings as " << num_mappings;

	avail = file_len - is.tellg();
	if (avail < Mapping::LENGTH * num_mappings) {
		BOOST_LOG_TRIVIAL(warning) << "[coloring] needed " << Mapping::LENGTH * num_mappings << "bytes of mapping data, but only " << avail << " available, aborting.";
		is.close();
		return;
	}

	if (num_mappings > 0) {
		for (int i = 0; i < num_mappings; i++) {
			Mapping *mapping = new Mapping(is);
			mappings[mapping->checksum]=mapping;
		}
	}
	else if (avail > 0) {
		BOOST_LOG_TRIVIAL(warning) << "[coloring] no mappings defined, but still " << avail << "bytes in the file!";
		is.close();
		return;
	}

	int num_masks = read_u8(is);
	BOOST_LOG_TRIVIAL(trace) << "[coloring] offset " << is.tellg() << " read number of masks as " << num_masks;
	if (num_masks > 0) {
		int mask_bytes = (int)(file_len-is.tellg()) / num_masks;

		if (mask_bytes != 256 && mask_bytes != 512 && mask_bytes != 1536) {
			BOOST_LOG_TRIVIAL(warning) << "[coloring] " << num_masks << " with " << mask_bytes << "bytes/masks remaining, unknown size, ignoring";
			is.close();
			return;
		}

		masks = new uint8_t[num_masks*mask_bytes];
		is.read((char*)masks, num_masks * mask_bytes);
		BOOST_LOG_TRIVIAL(trace) << "[coloring] read " << num_masks << " masks with " << mask_bytes << " bytes per mask";
	}

	avail = file_len - is.tellg();
	if (avail) {
		BOOST_LOG_TRIVIAL(warning) << "[coloring] erad error, finished reading, but still " << avail << " bytes left";
	}

	is.close();
}

Palette* Coloring::get_palette(uint32_t index)
{
	return nullptr;
}

Mapping* Coloring::find_mapping(uint32_t checksum)
{
	return NULL;
}
