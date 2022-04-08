#include <filesystem>

#include "pin2dmdcolorisation.h"
#include "animationset.h"
#include "animation.h"
#include "palmapping.h"

#include "../dmd/palette_colorizer.h"
#include "../util/crc32.h"

// We can enable/disbale up to 8 license features
#define LICENSE_VNI 0x01

uint8_t license_features(const vector<uint8_t> hostid, const vector<uint8_t> license_code) {
	return 0xff;
}

string to_hex(const vector<uint8_t> data) {
	string res;
	for (auto x : data) {
		std::ostringstream oss;
		oss << std::hex << std::setw(2) << std::setfill('0') << (unsigned)x;
		res += oss.str();
	}
	return res;
}

bool Pin2DMDColorisation::configure_from_ptree(boost::property_tree::ptree pt_general, boost::property_tree::ptree pt_source)
{
	// The host id for all licensing stuff
	vector<uint8_t> hostid = { 0,0,0,0,0,0,0,0 };
	vector<uint8_t> license_code = { 0,0,0,0,0,0,0,0 };

	BOOST_LOG_TRIVIAL(error) << "[pin2dmdcolorisation] hostid is " << to_hex(hostid);

	uint8_t features = license_features(hostid, license_code);

	bool ok = false;

	string basename = pt_source.get("basename", "");
	if (basename == "") {
		BOOST_LOG_TRIVIAL(error) << "[pin2dmdcolorisation] basename has not been configured, aborting";
		return false;
	}

	if (features & LICENSE_VNI) {

		BOOST_LOG_TRIVIAL(trace) << "[pin2dmdcolorisation] VNI licensed";

		if (std::filesystem::exists(basename + ".pal") && std::filesystem::exists(basename + ".vni")) {
			BOOST_LOG_TRIVIAL(info) << "[pin2dmdcolorisation] using " << basename << ".pal/.vni";
		};

		coloring = PalColoring(basename + ".pal");
		animations = VniAnimationSet(basename + ".vni");

		ok = true;
	}

	if (! ok) {
		// TODO: pal/fsq
		BOOST_LOG_TRIVIAL(trace) << "[pin2dmdcolorisation] .pal/.fsq not yet implemented";
	}

	if (!ok) {
		BOOST_LOG_TRIVIAL(info) << "[pin2dmdcolorisation] couldn't load a colorisation, aborting";
	}

	// count animations and frames
	src_frame_count = src_current_animation = src_current_frame = 0;
	for (auto a : animations.animations) {
		src_frame_count += a.second->frames.size();
	}

	// Set default palette
	col_palette = coloring.palettes[coloring.default_palette_index]->colors;

	return true;
}

DMDFrame Pin2DMDColorisation::process_frame(DMDFrame &f)
{
	if (f.is_null()) {
		BOOST_LOG_TRIVIAL(info) << "[frameinfologger] got NULL frame doing nothing";
		return f;
	}

	uint32_t chk;
	BOOST_LOG_TRIVIAL(trace) << "[vnicolorisation] got frame " << f.get_width() << "x" << f.get_height() << " " << f.get_bitsperpixel() << "bpp, checksum " << f.get_checksum();
	int w = f.get_width();
	int h = f.get_height();
	int len = w * h;
	int plane_len = len / 8;

	PaletteMapping* map = NULL;
	uint8_t* color_data = new uint8_t[(int)len * 3];

	// find colormapping
	for (int i = 0; i < f.get_bitsperpixel(); i++) {
		vector<uint8_t> pd = f.get_plane(i);

		uint32_t chk = crc32vect(pd,true);
		BOOST_LOG_TRIVIAL(trace) << "[vnicolorisation] plane " << std::hex << i << " crc32(full frame) " << chk;
		map = coloring.find_mapping(chk);

		if (map) {
			BOOST_LOG_TRIVIAL(trace) << "[vnicolorisation] found colormapping for unmasked frame";
		}
		else {
			// try to find an colormapping that matches
			for (uint8_t* mask : coloring.masks) {
				// TODO: This is a hack, change mask from uint8_t* to vector<uint8_t> later
				vector <uint8_t> maskVect = vector<uint8_t>();
				for (int i = 0; i < plane_len; i++) {
					maskVect.push_back(mask[i]);
				}

				chk = crc32vect(pd, maskVect, true);
				BOOST_LOG_TRIVIAL(trace) << "[vnicolorisation] plane masked crc32(full frame) " << chk;
				map = coloring.find_mapping(chk);

				if (map) {
					BOOST_LOG_TRIVIAL(trace) << "[vnicolorisation] found colormapping on masked frame";
					break;
				}
			}
		}

		if (map) {
			uint16_t index = map->palette_index;

			col_palette = coloring.palettes[index]->colors;
			col_mode = map->mode;

			// Should the palette be used only for  specific number of frames?
			if (map->duration) {
				col_frames_left = map->duration;
				BOOST_LOG_TRIVIAL(trace) << "[vnicolorisation] switching to palette " << index << " for " << col_frames_left << " frames";
			}
			else {
				col_frames_left = -1;
				BOOST_LOG_TRIVIAL(trace) << "[vnicolorisation] switching to palette " << index;
			}

			if (map->IsAnimation()) {
				col_animation = animations.find(map->offset);
				if (col_animation) {
					BOOST_LOG_TRIVIAL(trace) << "[vnicolorisation] starting animation " << col_animation->name << " (offset " << map->offset << ")";
					col_anim_frame = 0;
				}
				else {
					BOOST_LOG_TRIVIAL(error) << "[vnicolorisation] animation with offset " << map->offset << " not found";
				}
			}
			else if (col_mode == ModePalette) {
				// stop animation if one if running
				col_animation = NULL;
			}
		}

		// delete[] pd;

		if (map) {
			break;
		}
	}

	// Play animation
	if (col_animation) {

		color_animation_frame(f, col_animation->frames[col_anim_frame], len, color_data);

		col_frames_left--;
		col_anim_frame++;
		if ((col_frames_left == 0) || (col_anim_frame >= col_animation->frames.size())) {
			col_animation = NULL;
			col_mode = ModePalette;
		}
	}
	else {
		AnimationFrame af;
		color_animation_frame(f, af, len, color_data);
	}

	DMDFrame res = DMDFrame(w, h, 24, color_data);

	return res;
}

DMDFrame Pin2DMDColorisation::next_frame(bool blocking)
{
	Animation *anim = animations.animations[src_current_animation];

	DMDFrame frame = anim->frames[src_current_frame_in_animation].as_dmd_frame(anim->width, anim->height);
	if (src_current_frame_in_animation < anim->frames.size() - 1) {
		src_current_frame_in_animation++;
	}
	else {
		src_current_frame_in_animation = 0;
		src_current_animation++;
	}
	src_current_frame++;

	return frame;
}

bool Pin2DMDColorisation::finished()
{
	return !(frame_ready());
}

bool Pin2DMDColorisation::frame_ready()
{
	return src_current_frame < src_frame_count;
}

SourceProperties Pin2DMDColorisation::get_properties()
{
	return SourceProperties(animations.max_width, animations.max_height, 8);
}

void Pin2DMDColorisation::color_animation_frame(DMDFrame &src_frame, AnimationFrame &anim_frame, int len, uint8_t* dst)
{
	if (col_mode == ModeEvent) {
		BOOST_LOG_TRIVIAL(error) << "[pin2dmdcolorisation] mode EVENT not supported, ignoring";
		return;
	}

	bool is_animation = (col_mode != ModeEvent) && (col_mode != ModePalette);

	uint8_t src_mask = 0xff >> (8 - src_frame.get_bitsperpixel());
	uint8_t color_mask = ~src_mask & 0x7f;

	auto anim_frame_data = anim_frame.get_frame_data();
	auto animIter = anim_frame_data.cbegin();

	// loop through pixels
	for (auto src_px: src_frame.get_data()) {

		DMDColor c;

		if (col_mode == ModeColorMask) {
			uint8_t ani_px = *animIter;
			animIter++;
			if (ani_px & 0x80) {
				uint8_t pv2 = src_px | (ani_px & color_mask);
				c = col_palette[pv2];
			}
			else {
				c = col_palette[src_px];
			}
		}
		else if ((col_mode == ModeReplace) || (col_mode == ModeFollowReplace)) {
			uint8_t ani_px = *animIter;
			animIter++;
			if (ani_px & 0x80) {
				uint8_t pv2 = ani_px & 0x7f;
				c = col_palette[pv2];
			}
			else {
				c = col_palette[src_px];
			}
		}
		else if (col_mode == ModeLayeredColorMask || (col_mode == ModeMaskedReplace) ) {
			// Not sure if this is correct
			uint8_t ani_px = *animIter;
			animIter++;
			if (ani_px & 0x80) {
				uint8_t pv2 = src_px | (ani_px & color_mask);
				c = col_palette[pv2];
			}
			else {
				c = col_palette[src_px];
			}
		}		else if (col_mode == ModePalette) {
			c = col_palette[src_px];
		}
		else {
			BOOST_LOG_TRIVIAL(info) << "[pin2dmdcolorisation] mode " << col_mode << " not supported, aborting";
			assert(false);
		}

		*dst = c.c.cols.r;
		dst++;
		*dst = c.c.cols.g;
		dst++;
		*dst = c.c.cols.b;
		dst++;
	}
}
