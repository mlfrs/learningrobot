/*
 * Authors: Cormac Guerin
 *
 * Name: mlfrs_simbody.h
 *
 */

#ifndef _MLFRS_SIMBODY_H_
#define _MLFRS_SIMBODY_H_

#include "../model/mdl_parser.h"
#include "../object_node.h"
#include "Simbody.h"
#include "physics_engine.h"
#include <vector>

class MlfrsSimbody : public PhysicsEngine {

private:
	SimTK::MultibodySystem simbody_sys;
	SimTK::SimbodyMatterSubsystem matter;
	SimTK::GeneralForceSubsystem forces;
	SimTK::ContactTrackerSubsystem tracker;
	SimTK::CompliantContactSubsystem contact;
	SimTK::Force::UniformGravity gravity;
	// SimTK::Integrator integ;
	SimTK::CPodesIntegrator integ;

	struct simbodyObject {
		SimTK::MobilizedBody mBody;
		std::string type;
		int id;
	};
	std::vector<simbodyObject> simbodyObjects;

	void visualize();
	void realize();
	void init();

public:
	MlfrsSimbody();
	~MlfrsSimbody();

	void createBox(MdlParser::mdl_object object, ObjectNode objectNode);
	void createSphere(MdlParser::mdl_object object, ObjectNode objectNode);
	void createCylinder(MdlParser::mdl_object object, ObjectNode objectNode);
	ObjectNode getStatefulObjectNode(ObjectNode objectNode);
	ObjectNode buildObjectNode(ObjectNode objectNode, SimTK::MobilizedBody mBody);
	void printContactSurfaces();
	void run();

};

#endif

