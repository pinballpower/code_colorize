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

uint8_t* VniAnimationFrame::get_frame_data()
{
	return frame_data;
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
			BOOST_LOG_TRIVIAL(trace) << "[vinanimation] offset " << is.tellg() << " read plane ";
			VniAnimationPlane *plane = new VniAnimationPlane(is, plane_size, marker);
			planes.push_back((AnimationPlane*)plane);
		}
	}

	combine_planes(plane_size);
}

void VniAnimationFrame::combine_planes(int plane_size)
{
	delete[] frame_data;
	frame_data = new uint8_t[plane_size * 8];

	uint8_t* pp = frame_data;
	int byteinword = 0;

	// Loop through all bytes
	for (int i = 0; i < plane_size; i++) {

		// Loop through all bits (pixels)
		for (int j = 0; j < 8; j++) {

			uint8_t pixval = 0;
			// combine all planes
			for (int k = planes.size()-1; k >= 0; k--) {
			//for (int k = 0; k < 2; k++) {
				uint8_t px_data = planes[k]->plane[i];

				uint8_t pv = (px_data >> j) & 0x01; // extract n-th bit

				pixval = (pixval << 1) | pv;  // combine
			}

			// store pixel
			*pp = pixval;
			pp++;
		}
	}

	// TODO: Check ENDIAN of the system
	reverse_word_order_array((uint32_t*)frame_data, plane_size * 2);
}

