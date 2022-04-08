#include <boost/log/trivial.hpp>

#include "palmapping.h"
#include "switchmode.h"
#include "streamhelper.h"

bool PaletteMapping::IsAnimation()
{
	return ((mode != ModeEvent) && (mode != ModePalette));
}

PaletteMapping::PaletteMapping(istream& is)
{
	checksum = read_u32_be(is);
	BOOST_LOG_TRIVIAL(trace) << "[palettemapping] offset " << is.tellg() << " read checksum as " << std::hex << checksum;
	mode = (SwitchMode)read_u8(is);
	BOOST_LOG_TRIVIAL(trace) << "[palettemapping] offset " << is.tellg() << " read mode as " << mode;
	palette_index = read_u16_be(is);
	BOOST_LOG_TRIVIAL(trace) << "[palettemapping] offset " << is.tellg() << " read index as " << palette_index;
	if (mode == ModePalette) {
		duration = read_u32_be(is);
		BOOST_LOG_TRIVIAL(trace) << "[palettemapping] offset " << is.tellg() << " read duration as " << duration;
		offset = 0;
	} else {
		offset = read_u32_be(is);
		BOOST_LOG_TRIVIAL(trace) << "[palettemapping] offset " << is.tellg() << " read offset as " << offset;
		duration = 0;
	}
}
