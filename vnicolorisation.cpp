#include <filesystem>

#include "vnicolorisation.h"


VniColorisation::VniColorisation()
{
}

VniColorisation::~VniColorisation()
{
}

bool VniColorisation::configure_from_ptree(boost::property_tree::ptree pt_general, boost::property_tree::ptree pt_source)
{
	string basename = pt_source.get("basename", "");
	if (basename == "") {
		BOOST_LOG_TRIVIAL(error) << "vnicoloring basename has not been configured";
		return false;
	}

	if (!(std::filesystem::exists(basename + ".pal"))) {
		BOOST_LOG_TRIVIAL(error) << "vnicoloring " << basename << ".pal does not exist";
		return false;
	};

	delete coloring;
	coloring = new Coloring(basename + ".pal");

	if (!(std::filesystem::exists(basename + ".vni"))) {
		BOOST_LOG_TRIVIAL(error) << "vnicoloring " << basename << ".vni does not exist";
		return false;
	};

	delete animations;
	animations = new VniAnimationSet(basename + ".vni");

	// count animations and frames
	frame_count = current_animation = current_frame = 0;
	for (auto a : animations->animations) {
		frame_count += a->frames.size();
	}

}

DMDFrame* VniColorisation::process_frame(DMDFrame* f)
{
	// TODO
	return f;
}

DMDFrame* VniColorisation::next_frame(bool blocking)
{
	return nullptr;
	// TODO
}

bool VniColorisation::finished()
{
	return !(frame_ready());
}

bool VniColorisation::frame_ready()
{
	return current_frame < frame_count;
}

void VniColorisation::get_properties(SourceProperties* p)
{
	p->bitsperpixel = 8;
	if (animations) {
		p->width = animations->max_width;
		p->height = animations->max_height;
	}
	else {
		p->width = 0;
		p->height = 0;
	}
}