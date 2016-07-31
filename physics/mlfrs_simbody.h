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
	SimTK::TimeStepper timestep;

	struct simbodyObject {
		SimTK::MobilizedBody mBody;
		std::string type;
		int rigidBodyAid;
		int rigidBodyBid;
	};
	std::vector<simbodyObject> simbodyObjects;

	struct rigidBody {
		SimTK::Body::Rigid rBody;
		int id;
	};
	std::vector<rigidBody> rigidBodies;

	void visualize();
	void realize();
	void setup();
	void init();
	void ruu();

public:
	MlfrsSimbody();
	~MlfrsSimbody();

	void createBox(MdlParser::mdl_object object, ObjectNode objectNode);
	void createSphere(MdlParser::mdl_object object, ObjectNode objectNode);
	void createCylinder(MdlParser::mdl_object object, ObjectNode objectNode);
	ObjectNode getStatefulObjectNode(ObjectNode objectNode);
	ObjectNode buildObjectNode(ObjectNode objectNode, SimTK::MobilizedBody mBody);
	void createJoint(MdlParser::mdl_joint joint, ObjectNode objectNodeA, ObjectNode objectNodeB);
	void createFreeJoint(MdlParser::mdl_joint joint, ObjectNode objectNodeA, ObjectNode objectNodeB);
	void createBallJoint(MdlParser::mdl_joint joint, ObjectNode objectNodeA, ObjectNode objectNodeB);
	void printContactSurfaces();
	void run();

};

#endif

