#include <boost/log/trivial.hpp>

#include "vnianimation.h"
#include "vnianimationframe.h"
#include "streamhelper.h"
#include "bithelper.h"
#include "../dmd/color.h"

VniAnimation::VniAnimation(ifstream& is, int file_version) {

	// animations name
	int name_length = read_int16_be(is);
	name = name_length > 0 ? read_string(is, name_length) : "<undefined>";
	BOOST_LOG_TRIVIAL(trace) << "[vinanimation] offset " << is.tellg() << " read animation name as " << name;

	// other data (mostly unused)
	cycles = read_int16_be(is);
	hold = read_int16_be(is);
	clock_from = read_int16_be(is);
	clock_small = (read_u8(is) != 0);
	clock_in_front = read_u8(is) != 0;
	clock_offsetX = read_int16_be(is);
	clock_offsetY = read_int16_be(is);
	refresh_delay = read_int16_be(is);
	type = read_u8(is);
	fsk = read_u8(is);

	int num_frames = read_int16_be(is);
	if (num_frames < 0) {
		num_frames += 65536;
	}
	BOOST_LOG_TRIVIAL(trace) << "[vinanimation] offset " << is.tellg() << " read animation frame count as " << num_frames;

	if (file_version >= 2) {
		read_palettes_and_colors(is);
	}
	if (file_version >= 3) {
		edit_mode = (AnimationEditMode)read_u8(is);
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
		VniAnimationFrame* frame = new VniAnimationFrame(is, file_version, animation_duration);

		if (frame->mask != NULL && transition_from == 0) {
			transition_from = i;
		}
		animation_duration += frame->delay;

		frames.push_back(frame);
	}
};

void VniAnimation::read_palettes_and_colors(ifstream& is)

{
	palette_index = read_int16_be(is);
	int num_colors = read_int16_be(is);
	if (num_colors <= 0) {
		return;
	}
	animation_colors = new DMDColor[num_colors];
	for (int i = 0; i < num_colors; i++) {
		animation_colors[i] = DMDColor(read_u8(is), read_u8(is), read_u8(is));
	}
	BOOST_LOG_TRIVIAL(trace) << "[vinanimation] offset " << is.tellg() << " found " << num_colors << " for palette " << palette_index;
};

