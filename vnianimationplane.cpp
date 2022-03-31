#include <cstdint>
#include <fstream>

#include <boost/log/trivial.hpp>

#include "vnianimationplane.h"
#include "bithelper.h"

VniAnimationPlane::VniAnimationPlane(std::ifstream& is, int plane_size, uint8_t marker)
{
	this->marker = marker;
	plane = new uint8_t[plane_size];
	is.read((char*)plane, plane_size);
	reverse_byte_array(plane, plane_size);
	BOOST_LOG_TRIVIAL(trace) << "[vinanimationplane] offset " << is.tellg() << " read animation plane, length " << plane_size;
};

VniAnimationPlane::~VniAnimationPlane()
{
	delete[] plane;
};
