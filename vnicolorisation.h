#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/log/trivial.hpp>

#include "../dmd/frameprocessor.h"
#include "../dmd/dmdframe.h"
#include "../dmdsource/dmdsource.h"
#include "coloring.h"
#include "vnianimationset.h"

class VniColorisation : DMDFrameProcessor, DMDSource {

public:

	VniColorisation();
	~VniColorisation();
	virtual bool configure_from_ptree(boost::property_tree::ptree pt_general, boost::property_tree::ptree pt_source);

	virtual DMDFrame* process_frame(DMDFrame* f);

	// DMDSource methods
	virtual DMDFrame* next_frame(bool blocking = true);
	virtual bool finished();
	virtual bool frame_ready();
	virtual void get_properties(SourceProperties* p);

private:

	Coloring* coloring = NULL;
	VniAnimationSet* animations = NULL;

	// for use as a DMDSource
	int current_animation = 0;
	int current_frame = 0;
	int current_frame_in_animation = 0;

	/// <summary>
	/// Number of all frames in all animations
	/// </summary>
	int frame_count;
};