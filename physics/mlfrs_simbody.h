/*
 * Authors: Cormac Guerin
 *
 * Name: mlfrs_simbody.h
 *
 */

#ifndef _MLFRS_SIMBODY_H_
#define _MLFRS_SIMBODY_H_

#include "../model/mdl_parser.h"
#include "physics_body.h"
#include "Simbody.h"
#include <vector>

class MlfrsSimbody 
{
private:
	SimTK::MultibodySystem system;
	SimTK::SimbodyMatterSubsystem matter;
	SimTK::GeneralForceSubsystem forces;
	SimTK::Force::Gravity gravity;

public:
	MlfrsSimbody();
	~MlfrsSimbody();

	void createSphere(MdlParser::mdl_object object, PhysicsBody* physicsBody);
	void init();
	void visualize();
	void realize();
	void run();

};

#endif

