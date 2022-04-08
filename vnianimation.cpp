#include <boost/log/trivial.hpp>

#include "vnianimation.h"
#include "vnianimationframe.h"
#include "streamhelper.h"
#include "../util/bithelper.h"
#include "../dmd/color.h"

VniAnimation::VniAnimation(ifstream& is, int file_version) {

	// offset
	offset = is.tellg();

	// animations name
	int name_length = read_int16_be(is);
	name = name_length > 0 ? read_string(is, name_length) : "<undefined>";
	BOOST_LOG_TRIVIAL(trace) << "[vinanimation] offset " << is.tellg() << " read animation name as " << name;

	// skip 16 bytes of unused data
	is.ignore(16);

	int num_frames = read_int16_be(is);
	if (num_frames < 0) {
		num_frames += 65536;
	}
	BOOST_LOG_TRIVIAL(trace) << "[vinanimation] offset " << is.tellg() << " read animation frame count as " << num_frames;

	if (file_version >= 2) {
		read_palettes_and_colors(is);
	}
	if (file_version >= 3) {
		read_u8(is); // AnimationEditMode not used
	}
	if (file_version >= 4) {
		width = read_int16_be(is);
		height = read_int16_be(is);
		BOOST_LOG_TRIVIAL(trace) << "[vinanimation] offset " << is.tellg() << " read dimension as " << width << "x" << height;
	}
	if (file_version >= 5)
	{
		int num_masks = read_int16_be(is);
		BOOST_LOG_TRIVIAL(trace) << "[vinanimation] offset " << is.tellg() << " read mask count as " << num_masks;
		for (int i = 0; i < num_masks; i++)
		{
			int locked = read_u8(is);
			int size = read_int16_be(is);
			uint8_t* m = new uint8_t[size];
			is.read((char*)m, size);
			reverse_byte_array(m, size);
			masks.push_back(m);
		}
	}

	if (file_version >= 6)
	{
		int linked_animation = read_u8(is);
		int size = read_int16_be(is);
		string anim_name = size > 0 ? read_string(is, size) : "<undefined>";
		uint32_t startFrame = read_u32_be(is);
		BOOST_LOG_TRIVIAL(trace) << "[vinanimation] offset " << is.tellg() << " read anim name " << anim_name;
	}

	BOOST_LOG_TRIVIAL(trace) << "[vinanimation] offset " << is.tellg() << " reading " << num_frames << " frame(s) for animation " << name;

	animation_duration = 0;
	for (int i = 0; i < num_frames; i++) {
		VniAnimationFrame frame = VniAnimationFrame(is, file_version, animation_duration);
		animation_duration += frame.delay;

		frames.push_back(frame);
	}
};

void VniAnimation::read_palettes_and_colors(ifstream& is)

{
	read_int16_be(is); // palette index unused
	int num_colors = read_int16_be(is);
	if (num_colors <= 0) {
		return;
	}

	BOOST_LOG_TRIVIAL(trace) << "[vinanimation] offset " << is.tellg() << " skipping " << num_colors*3 << " byte of embedded palette data ";
	is.ignore(num_colors * 3); // embedded colors unused
};

