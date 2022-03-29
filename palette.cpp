#include <iostream>
#include <fstream>

#include <boost/log/trivial.hpp>

#include "palette.h"
#include "streamhelper.h"
#include "../dmd/color.h"

using namespace std;

Palette::Palette() {
	index=0;
	type = 0;
	colors=new DMDColor[0];
}

Palette::Palette (istream& is) {
	index = read_u16_be(is);
	BOOST_LOG_TRIVIAL(trace) << "[palette] offset " << is.tellg() << " read index as " << index;
	int num_colors = read_u16_be(is);
	BOOST_LOG_TRIVIAL(trace) << "[palette] offset " << is.tellg() << " read numcolors as " << num_colors;
	type = read_u8(is);
	BOOST_LOG_TRIVIAL(trace) << "[palette] offset " << is.tellg() << " read type as " << ++type;
	colors = new DMDColor[num_colors];
	for (int i = 0, j=0; i < num_colors; i++, j+=3) {
		colors[i] = DMDColor(read_u8(is), read_u8(is), read_u8(is));
	}
	BOOST_LOG_TRIVIAL(trace) << "[palette] offset " << is.tellg() << " read " << num_colors*3 << " bytes of color data ((" << num_colors << " colors)";
}

Palette::Palette(DMDColor colors[])
{
}



bool Palette::is_default()
{
	return (type == 1 || type == 2);
}

bool Palette::is_persistent()
{
	return (type == 1);
}