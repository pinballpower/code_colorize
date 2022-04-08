#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/log/trivial.hpp>

#include "../dmd/frameprocessor.h"
#include "../dmd/dmdframe.h"
#include "../dmdsource/dmdsource.h"
#include "palcoloring.h"
#include "vnianimationset.h"

class Pin2DMDColorisation : DMDFrameProcessor, DMDSource {

public:

	virtual bool configure_from_ptree(boost::property_tree::ptree pt_general, boost::property_tree::ptree pt_source);

	virtual DMDFrame process_frame(DMDFrame &f);

	// DMDSource methods
	DMDFrame next_frame(bool blocking = true);
	virtual bool finished();
	virtual bool frame_ready();
	virtual SourceProperties get_properties();

private:

	PalColoring coloring;
	VniAnimationSet animations;

	void color_animation_frame(DMDFrame &src_frame, AnimationFrame &anim_frame, int len, uint8_t *dst);

	// for use as a DMDSource
	int src_current_animation = 0;
	int src_current_frame = 0;
	int src_current_frame_in_animation = 0;

	/// <summary>
	/// Number of all frames in all animations
	/// </summary>
	int src_frame_count;

	//
	// Variables for coloring
	//
	vector<DMDColor> col_palette;
	int col_frames_left = -1;
	Animation* col_animation = NULL;
	int col_anim_frame = 0;
	SwitchMode col_mode = ModePalette;

	// Features defined by the license
	bool allow_vni = false;
};