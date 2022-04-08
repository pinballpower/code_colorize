#include <cstdint>
#include <fstream>

#include <boost/log/trivial.hpp>

#include "vnianimationplane.h"
#include "../util/bithelper.h"
#include "../util/crc32.h"

VniAnimationPlane::VniAnimationPlane(std::ifstream& is, int plane_size, uint8_t marker)
{
	this->marker = marker;
	plane = new uint8_t[plane_size];
	is.read((char*)plane, plane_size);
	//reverse_byte_array(plane, plane_size);
	uint32_t chk = crc32buf(plane, plane_size, true);
	BOOST_LOG_TRIVIAL(trace) << "[vinanimationplane] offset " << is.tellg() << " read animation plane " << marker+0 << ", length " << plane_size << std::hex << " checksum " << chk;
};

VniAnimationPlane::~VniAnimationPlane()
{
	delete[] plane;
};
