/*
 * Authors: Cormac Guerin
 *
 * Name: mlfrs_simbody.cc
 *
 * The main simbody wrapper containing the functions 
 * we need create our physics simulation.
 *
 */

#include "mlfrs_simbody.h"
#include "Simbody.h"
#include "SimTKsimbody.h"
#include "simbody/internal/ForceSubsystemGuts.h"

using namespace SimTK;

static SimTK::MobilizedBody::Pin* placeHolder;
static SimTK::State state;

MlfrsSimbody::MlfrsSimbody() :	matter(simbody_sys), 
								forces(simbody_sys), 
								gravity(forces, matter, SimTK::Vec3(0, -9.81, 0))
								{}

// const System::Guts &ggg = simbody_sys.getSystemGuts();

void MlfrsSimbody::createBox(MdlParser::mdl_object object, MobilizedBody physicsBody) {
	try {
	    Body::Rigid bodyInfo(MassProperties(object.mass, Vec3(0), UnitInertia(1)));
		bodyInfo.addDecoration(Transform(), DecorativeBrick(
			Vec3(object.dimension[0],object.dimension[1],object.dimension[2])));
		physicsBody = MobilizedBody::Free(matter.Ground(), Transform(Vec3(0)),
			bodyInfo, Transform(Vec3(object.position[0],object.position[1],object.position[2])));
		simbodyObject sbo = { physicsBody, "free" };
	 	simbodyObjects.push_back(sbo);
	} catch (const std::exception& e) {
		std::cout << "Error in createSphere() : " << e.what() << std::endl;
		exit;
	}
}

void MlfrsSimbody::createSphere(MdlParser::mdl_object object, MobilizedBody physicsBody) {
	try {
	    Body::Rigid bodyInfo(MassProperties(object.mass, Vec3(0), UnitInertia(1)));
		bodyInfo.addDecoration(Transform(), DecorativeSphere(object.dimension[0]));
		physicsBody = MobilizedBody::Free(matter.Ground(), Transform(Vec3(0)),
			bodyInfo, Transform(Vec3(object.position[0],object.position[1],object.position[2])));
		simbodyObject sbo = { physicsBody, "free" };
	 	simbodyObjects.push_back(sbo);
	} catch (const std::exception& e) {
		std::cout << "Error in createSphere() : " << e.what() << std::endl;
		exit;
	}
}

void MlfrsSimbody::createCylinder(MdlParser::mdl_object object, MobilizedBody physicsBody) {
	try {
	    Body::Rigid bodyInfo(MassProperties(object.mass, Vec3(0), UnitInertia(1)));
		bodyInfo.addDecoration(Transform(), DecorativeCylinder(
			object.dimension[0],object.dimension[1]));
		physicsBody = MobilizedBody::Free(matter.Ground(), Transform(Vec3(0)),
			bodyInfo, Transform(Vec3(object.position[0],object.position[1],object.position[2])));
		simbodyObject sbo = { physicsBody, "free" };
	 	simbodyObjects.push_back(sbo);
	} catch (const std::exception& e) {
		std::cout << "Error in createSphere() : " << e.what() << std::endl;
		exit;
	}
}

void MlfrsSimbody::visualize() {
	try {
		Visualizer viz(simbody_sys);
		simbody_sys.addEventReporter(new Visualizer::Reporter(viz, 0.01));
	} catch (const std::exception& e) {
		exit;
	}
}

void MlfrsSimbody::realize() {
	try {
		state = simbody_sys.realizeTopology();
	} catch  (const std::exception& e) {
		std::cout << "Error in realize() : " << e.what() << std::endl;
		exit;
	}
}

void MlfrsSimbody::run() {
	try {
		// Here we iterate over all simbody objects and apply any extra runtime parameters.
		for(std::vector<simbodyObject>::iterator it = simbodyObjects.begin(); it != simbodyObjects.end(); ++it) {
			if ((*it).type == "pin") {
				MobilizedBody::Pin m = MobilizedBody::Pin::downcast((*it).mBody);
				m.setRate(state, 5.0);
			}
		}
	    RungeKuttaMersonIntegrator integ(simbody_sys);
	    TimeStepper ts(simbody_sys, integ);
	    ts.initialize(state);
		ts.stepTo(20.0);
	} catch  (const std::exception& e) {
		exit;
	}
}

MlfrsSimbody::~MlfrsSimbody() {}

