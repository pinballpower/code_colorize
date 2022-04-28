#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/log/trivial.hpp>

#include "../processor/frameprocessor.h"
#include "../dmd/dmdframe.h"
#include "../dmdsource/dmdsource.h"
#include "palcoloring.h"
#include "vnianimationset.h"

class Pin2DMDColorisation : public DMDFrameProcessor, public DMDSource {

public:

	virtual bool configureFromPtree(boost::property_tree::ptree pt_general, boost::property_tree::ptree pt_source);

	virtual DMDFrame process_frame(DMDFrame &f);

	// DMDSource methods
	// DMDFrame next_frame(bool blocking = true);
	virtual bool isFinished();
	virtual bool isFrameReady();
	virtual SourceProperties getProperties();

private:

	PalColoring coloring;
	VniAnimationSet animations;

	vector <uint8_t> color_animation_frame(const DMDFrame &src_frame, const AnimationFrame &anim_frame, int len);

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
	Animation col_animation;
	bool animation_active = false;
	int col_anim_frame = 0;
	SwitchMode col_mode = ModePalette;

	// Features defined by the license
	bool allow_vni = false;
};