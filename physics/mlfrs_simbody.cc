#include "mlfrs_simbody.h"

using namespace SimTK;

MlfrsSimbody::MlfrsSimbody()
{
}

void MlfrsSimbody::init() {

	MultibodySystem system;
	SimbodyMatterSubsystem matter(system);
	GeneralForceSubsystem forces(system);
	Force::Gravity gravity(forces, matter, -YAxis, 9.8);
	
	Body::Rigid bodyInfo(MassProperties(1.0, Vec3(0), UnitInertia(1)));
	bodyInfo.addDecoration(Transform(), DecorativeSphere(0.1));

	MobilizedBody::Pin pendulum1(matter.Ground(), Transform(Vec3(0)),
		bodyInfo, Transform(Vec3(0, 1, 0)));
    MobilizedBody::Pin pendulum2(pendulum1, Transform(Vec3(0)),
		bodyInfo, Transform(Vec3(0, 1, 0)));
	
	Visualizer viz(system);
    system.addEventReporter(new Visualizer::Reporter(viz, 0.01));

	State state = system.realizeTopology();
    pendulum2.setRate(state, 5.0);

	RungeKuttaMersonIntegrator integ(system);
    TimeStepper ts(system, integ);
    ts.initialize(state);
    ts.stepTo(20.0);

}

MlfrsSimbody::~MlfrsSimbody()
{
}
