/*
 * Authors: Cormac Guerin
 *
 * Name: mlfrs_simbody.h
 *
 */

#ifndef _MLFRS_SIMBODY_H_
#define _MLFRS_SIMBODY_H_

#include "../model/mdl_parser.h"
#include "Simbody.h"
#include "physics_engine.h"
#include <vector>

class MlfrsSimbody : public PhysicsEngine {

private:
	SimTK::MultibodySystem simbody_sys;
	SimTK::SimbodyMatterSubsystem matter;
	SimTK::GeneralForceSubsystem forces;
	SimTK::Force::Gravity gravity;

	struct simbodyObject {
		SimTK::MobilizedBody mBody;
		std::string type;
	};
	std::vector<simbodyObject> simbodyObjects;

public:
	MlfrsSimbody();
	~MlfrsSimbody();

	void createBox(MdlParser::mdl_object object, SimTK::MobilizedBody physicsBody);
	void createSphere(MdlParser::mdl_object object, SimTK::MobilizedBody physicsBody);
	void createCylinder(MdlParser::mdl_object object, SimTK::MobilizedBody physicsBody);
	void visualize();
	void realize();
	void run();

	SimTK::MobilizedBody simBody;
	virtual SimTK::MobilizedBody getBody() { return simBody; }

};

#endif

