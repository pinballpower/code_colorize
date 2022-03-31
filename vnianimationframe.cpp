#include <cassert>

#include <boost/log/trivial.hpp>

#include "streamhelper.h"
#include "bithelper.h"
#include "vnianimationframe.h"
#include "vnianimationplane.h"


VniAnimationFrame::VniAnimationFrame(ifstream& is, int file_version, unsigned int time)
{
	int plane_size = read_int16_be(is);
	BOOST_LOG_TRIVIAL(trace) << "[vinanimationframe] offset " << is.tellg() << " plane_size is " << plane_size;
	delay = read_u16_be(is);

	if (file_version >= 4) {
		hash = read_u32_be(is);
	}

	bit_length = read_u8(is);
	BOOST_LOG_TRIVIAL(trace) << "[vinanimationframe] offset " << is.tellg() << " bit_length is " << bit_length;
	assert(bit_length <= 8);

	if (file_version < 3) {
		read_planes(is, plane_size);
	}
	else {
		bool compressed = (read_u8(is) != 0);
		if (!compressed) {
			read_planes(is, plane_size);

		}
		else {
			BOOST_LOG_TRIVIAL(error) << "[vinanimation] compressed planed not yet supported";
			// TODO: implement compressed planes
		}
	}
}

VniAnimationFrame::~VniAnimationFrame()
{
	// Delete all planes
	for (auto p : planes)
	{
		delete p;
	}
	planes.clear();
}

void VniAnimationFrame::read_planes(ifstream& is, int plane_size)
{
	for (int i = 0; i < bit_length; i++) {
		uint8_t marker = read_u8(is);
		if (marker == 0x6d) {
			BOOST_LOG_TRIVIAL(trace) << "[vinanimation] offset " << is.tellg() << " read mask";
			mask = new uint8_t[plane_size];
			is.read((char*)mask, plane_size);
			reverse_byte_array(mask, plane_size);
		}
		else {
			VniAnimationPlane *plane = new VniAnimationPlane(is, plane_size, marker);
			planes.push_back((AnimationPlane*)plane);
		}
	}
}
