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

const float TRANSITION_VELOCITY = 1e-3;
const float GRAVITY = -90.81;
const float FRAME_RATE = 30;
const bool TRACK_DISSIPATED_ENERGY = true;
const Rotation R_xdown(-Pi/2,ZAxis);
const Rotation R_ydown(-Pi,ZAxis);
const Transform T_xdown(R_xdown, Vec3(0,-20,0));

// generic material. todo: these should be loaded externally.
ContactMaterial material(0.02*1e7, // stiffness
						 0.9,     // dissipation
						 0.8,     // mu_static
						 0.6,     // mu_dynamic
						 1); // mu_viscous

MlfrsSimbody::MlfrsSimbody() :	matter(simbody_sys), 
								forces(simbody_sys), 
								tracker(simbody_sys),
								contact(simbody_sys, tracker),
								gravity(forces, matter, Vec3(0, GRAVITY, 0))
								{}

// const System::Guts &ggg = simbody_sys.getSystemGuts();

void MlfrsSimbody::createBox(MdlParser::mdl_object object, MobilizedBody physicsBody) {
	try {
	    Body::Rigid bodyInfo(MassProperties(object.mass, Vec3(0), UnitInertia(1)));
		bodyInfo.addDecoration(Transform(), DecorativeBrick(
				Vec3(object.dimension[0], object.dimension[1], object.dimension[2]))
			.setOpacity(0.4));
		bodyInfo.addContactSurface(Transform(),
				ContactSurface(ContactGeometry::Brick(
					Vec3(object.dimension[0], object.dimension[1], object.dimension[2])),
				material)
			.joinClique(ContactSurface::createNewContactClique()));
		physicsBody = MobilizedBody::Free(matter.Ground(), Transform(Vec3(0)),
			bodyInfo, Transform( R_ydown, 
			Vec3(object.position[0], object.position[1], object.position[2])));
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
		bodyInfo.addDecoration(Transform(), 
			DecorativeSphere(object.dimension[0]).setOpacity(0.4));
		bodyInfo.addContactSurface(Transform(),
				ContactSurface(ContactGeometry::Sphere(object.dimension[0]), material)
			.joinClique(ContactSurface::createNewContactClique()));
		physicsBody = MobilizedBody::Free(matter.Ground(), Transform(Vec3(0)),
			bodyInfo, Transform( R_ydown,
				Vec3(object.position[0], object.position[1], object.position[2])));
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
			object.dimension[0], object.dimension[1]).setOpacity(0.4));
		// looks like they don't have a proper cylinder contact body..
		// use brick instead
		bodyInfo.addContactSurface(Transform(),
			ContactSurface(ContactGeometry::Brick(Vec3(
				object.dimension[0], object.dimension[1], object.dimension[2])), material)
			.joinClique(ContactSurface::createNewContactClique()));
		physicsBody = MobilizedBody::Free(matter.Ground(), Transform(Vec3(0)),
			bodyInfo, Transform( R_ydown, 
			Vec3(object.position[0],object.position[1],object.position[2])));
		simbodyObject sbo = { physicsBody, "free" };
	 	simbodyObjects.push_back(sbo);
	} catch (const std::exception& e) {
		std::cout << "Error in createSphere() : " << e.what() << std::endl;
		exit;
	}
}

// configure the engine/scene
void MlfrsSimbody::init() {
	// setup contact
	contact.setTrackDissipatedEnergy(TRACK_DISSIPATED_ENERGY);
    contact.setTransitionVelocity(TRANSITION_VELOCITY);
	// setup the ground
	matter.Ground().updBody().addContactSurface(
		// Rotation(Pi/2,ZAxis),
		// Rotation(R_xdown,YAxis),
		Transform(R_xdown, Vec3(0, 0, 0)),
		ContactSurface(ContactGeometry::HalfSpace(), material)
	.joinClique(ContactSurface::createNewContactClique()));
}

void MlfrsSimbody::printContactSurfaces() {
	for (MobilizedBodyIndex mbx(0); mbx < matter.getNumBodies(); ++mbx) {
		const MobilizedBody& mBody = matter.getMobilizedBody(mbx);
		const int nsurfs = mBody.getBody().getNumContactSurfaces();
		std::cout << "Movable body " << (int)mbx << " has " << nsurfs << " contact surfaces" << std::endl;
		for (int i=0; i<nsurfs; ++i) {
//			std::cout << " " << i << " - index : " << (int)tracker.getContactSurfaceIndex(mbx,i) << std::endl;
		}
	}
}

void MlfrsSimbody::visualize() {
	try {
		Visualizer viz(simbody_sys);
		viz.setMode(Visualizer::RealTime);
		viz.setDesiredBufferLengthInSec(1);
		viz.setDesiredFrameRate(FRAME_RATE);
		viz.setGroundHeight(0);
		viz.setShowShadows(true);
		simbody_sys.addEventReporter(new Visualizer::Reporter(viz, 0.01));
	} catch (const std::exception& e) {
		exit;
	}
}

void MlfrsSimbody::realize() {
	try {
		state = simbody_sys.realizeTopology();
	//	state = system.getDefaultState();
	} catch  (const std::exception& e) {
		std::cout << "Error in realize() : " << e.what() << std::endl;
		exit;
	}
}

void MlfrsSimbody::run() {
	try {

		visualize();
		realize();

		// Here we iterate over all simbody objects and apply any extra runtime parameters.
		for(std::vector<simbodyObject>::iterator it = simbodyObjects.begin(); it != simbodyObjects.end(); ++it) {
			if ((*it).type == "pin") {
				MobilizedBody::Pin m = MobilizedBody::Pin::downcast((*it).mBody);
				m.setRate(state, 5.0);
			}
		}

		// ExplicitEulerIntegrator integ(simbody_sys);
		CPodesIntegrator integ(simbody_sys,CPodes::BDF,CPodes::Newton);
		// RungeKuttaFeldbergIntegrator integ(simbody_sys);
		// RungeKuttaMersonIntegrator integ(simbody_sys);
		// RungeKutta3Integrator integ(simbody_sys);
		// VerletIntegrator integ(simbody_sys);
		// integ.setMaximumStepSize(1e-0001);
		integ.setAccuracy(1e-3); // minimum for CPodes
		// integ.setAccuracy(.01);
		// state = simbody_sys.getDefaultState();

	    TimeStepper ts(simbody_sys, integ);
	    ts.initialize(state);
		ts.stepTo(20.0);
	} catch  (const std::exception& e) {
		exit;
	}
}

MlfrsSimbody::~MlfrsSimbody() {}

