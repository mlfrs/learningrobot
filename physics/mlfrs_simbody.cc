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

using namespace SimTK;

static SimTK::MobilizedBody::Pin *placeHolder;
static SimTK::State state;

MlfrsSimbody::MlfrsSimbody() : matter(system), 
				 forces(system), 
				 gravity(forces, matter, SimTK::Vec3(0, -9.81, 0))
				 {}

void MlfrsSimbody::createSphere(MdlParser::mdl_object object, PhysicsBody* physicsBody) {


	Body::Rigid bodyInfo = physicsBody->getSimbody();
	bodyInfo.addDecoration(Transform(), DecorativeSphere(object.dimension[0]));

//	MobilizedBody::Free sphere(matter.Ground(), Transform(Vec3(0)),bodyInfo, Transform(Vec3(0, 1, 0)));
//	sphere.setDefaultTranslation(Vec3(object.position[0],object.position[1],object.position[2]));

	MobilizedBody::Pin pendulum1(matter.Ground(), Transform(Vec3(0)),
		bodyInfo, Transform(Vec3(0, 1, 0)));
  	MobilizedBody::Pin pendulum2(pendulum1, Transform(Vec3(0)),
		bodyInfo, Transform(Vec3(0, 1, 0)));

	Visualizer viz(system);
    system.addEventReporter(new Visualizer::Reporter(viz, 0.01));

	placeHolder=&pendulum2;

	// ISSUE: This is strange.. seems to only work if I execute it within this class/file . 
	// Some weird scoping issue??
	realize();
	run();
}

void MlfrsSimbody::visualize() {
	Visualizer viz(system);
    system.addEventReporter(new Visualizer::Reporter(viz, 0.01));
}

void MlfrsSimbody::realize() {
	state = system.realizeTopology();
}

void MlfrsSimbody::run() {
	
	placeHolder->setRate(state, 5.0);

    RungeKuttaMersonIntegrator integ(system);
    TimeStepper ts(system, integ);
    ts.initialize(state);
    ts.stepTo(20.0);
	
}

MlfrsSimbody::~MlfrsSimbody() {}

