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
const float GRAVITY = -9.81;
const float FRAME_RATE = 30;
const bool TRACK_DISSIPATED_ENERGY = true;
const Rotation rXdown(-Pi/2,ZAxis);
const Rotation rYdown(-Pi,ZAxis);

ContactCliqueId defaultClique = ContactSurface::createNewContactClique();

// generic material. todo: these should be loaded externally.
ContactMaterial material(0.02*1e7,	// stiffness
						 0.9,		// dissipation
						 0.8,		// mu_static
						 0.6,		// mu_dynamic
						 1);		// mu_viscous

MlfrsSimbody::MlfrsSimbody() :	matter(simbody_sys), 
								forces(simbody_sys), 
								tracker(simbody_sys),
								contact(simbody_sys, tracker),
								gravity(forces, matter, Vec3(0, GRAVITY, 0)),
								// ExplicitEulerIntegrator integ(simbody_sys);
								// CPodesIntegrator integ(simbody_sys,CPodes::BDF,CPodes::Newton);
								// RungeKuttaFeldbergIntegrator integ(simbody_sys);
								// RungeKuttaMersonIntegrator integ(simbody_sys);
								// RungeKutta3Integrator integ(simbody_sys);
								// VerletIntegrator integ(simbody_sys);
								integ(simbody_sys,CPodes::BDF,CPodes::Newton)
								{}

// const System::Guts &ggg = simbody_sys.getSystemGuts();

void MlfrsSimbody::createBox(MdlParser::mdl_object object, ObjectNode objectNode) {
	std::cout << "MlfrsSimbody::createBox - object : " << objectNode.oId << std::endl;
	try {
	    Body::Rigid bodyInfo(MassProperties(object.mass, Vec3(0), object.mass * UnitInertia::brick(
			Vec3(object.dimension[0],object.dimension[1],object.dimension[2]))));
		bodyInfo.addDecoration(Transform(), DecorativeBrick(
				Vec3(object.dimension[0], object.dimension[1], object.dimension[2]))
			.setOpacity(0.4));
		bodyInfo.addContactSurface(Transform(),
				ContactSurface(ContactGeometry::Brick(
					Vec3(object.dimension[0], object.dimension[1], object.dimension[2])),
				material)
		);
			//.joinClique(defaultClique));
		SimTK::MobilizedBody::Free physicsBody(matter.Ground(), Transform(Vec3(0)),
			bodyInfo, Transform( rYdown, 
			Vec3(object.position[0], object.position[1], object.position[2])));
		simbodyObject sbo = { physicsBody, "free", objectNode.oId };
	 	simbodyObjects.push_back(sbo);
	} catch (const std::exception& e) {
		std::cout << "Error in createBox() : " << e.what() << std::endl;
		exit;
	}
}

void MlfrsSimbody::createSphere(MdlParser::mdl_object object, ObjectNode objectNode) {
	std::cout << "MlfrsSimbody::createSphere - object : " << objectNode.oId << std::endl;
	try {
	    Body::Rigid bodyInfo(MassProperties(object.mass, Vec3(0), object.mass * UnitInertia::brick(
			Vec3(object.dimension[0]))));
		bodyInfo.addDecoration(Transform(), 
			DecorativeSphere(object.dimension[0]).setOpacity(0.4));
		bodyInfo.addContactSurface(Transform(),
				ContactSurface(ContactGeometry::Sphere(object.dimension[0]), material)
		);
			//.joinClique(defaultClique));
		SimTK::MobilizedBody::Free physicsBody(matter.Ground(), Transform(Vec3(0)),
			bodyInfo, Transform( rYdown,
				Vec3(object.position[0], object.position[1], object.position[2])));
		simbodyObject sbo = { physicsBody, "free", objectNode.oId };
	 	simbodyObjects.push_back(sbo);
	} catch (const std::exception& e) {
		std::cout << "Error in createSphere() : " << e.what() << std::endl;
		exit;
	}
}

void MlfrsSimbody::createCylinder(MdlParser::mdl_object object, ObjectNode objectNode) {
	std::cout << "MlfrsSimbody::createCylinder - object : " << objectNode.oId << std::endl;
	try {
	    Body::Rigid bodyInfo(MassProperties(object.mass, Vec3(0), object.mass * UnitInertia::brick(
			Vec3(object.dimension[0],object.dimension[1],object.dimension[2]))));
		bodyInfo.addDecoration(Transform(), DecorativeCylinder(
			object.dimension[0], object.dimension[1]).setOpacity(0.4));
		// looks like they don't have a proper cylinder contact body..
		// use brick instead
		bodyInfo.addContactSurface(Transform(),
			ContactSurface(ContactGeometry::Brick(Vec3(
				object.dimension[0], object.dimension[1], object.dimension[2])), material)
		);
			//.joinClique(defaultClique));
		SimTK::MobilizedBody::Free physicsBody(matter.Ground(), Transform(Vec3(0)),
			bodyInfo, Transform( rYdown, 
			Vec3(object.position[0],object.position[1],object.position[2])));
		simbodyObject sbo = { physicsBody, "free", objectNode.oId };
	 	simbodyObjects.push_back(sbo);
	} catch (const std::exception& e) {
		std::cout << "Error in createCylinder() : " << e.what() << std::endl;
		exit;
	}
}

/*
 * Takes an objectNode and returns an ObjectNode with the current state.
 */
ObjectNode MlfrsSimbody::getStatefulObjectNode(ObjectNode node) {
	for(std::vector<simbodyObject>::iterator it = simbodyObjects.begin();
		it != simbodyObjects.end(); ++it) {
		if ((*it).id == node.oId) {
			return buildObjectNode(node, (*it).mBody);
		}
	}
	std::cout << "Warning : Object '" << node.name << 
		"' with id " << node.oId << "not found" << std::endl;
	return ObjectNode();
}

/*
 * Function to return an object node with the current state.
 * State is read from the class Integrator 'integ'
 * The Location and Rotation(as angle) are read and
 * added to the object as Eigen3 Vectors.
 */
ObjectNode MlfrsSimbody::buildObjectNode(ObjectNode O, SimTK::MobilizedBody mBody) {

	// Get the state
	const SimTK::State &state_ = integ.getAdvancedState();
//	const SimTK::State &state_ = getAssembler().getInternalState();

	const Vec3 P = mBody.getBodyOriginLocation(state_);
	const Rotation R = mBody.getBodyRotation(state_);
	const Vec4 RA = R.convertRotationToAngleAxis();
	std::cout << R.convertRotationToAngleAxis() << std::endl;

	O.position=Eigen::Vector3f(P[0],P[1],P[2]);
	O.rotation=Eigen::Vector4f(RA[0],RA[1],RA[2],RA[3]);

	std::cout << " Eigen::Vector4f - " << O.position << std::endl;
	std::cout << " Eigen::Vector4f - " << O.rotation << std::endl;

	/*
	Mat33 A = R.toMat33();
	for (int i =0; i< 3;i++) {
		std::cout << A[i][0] << std::endl;
		std::cout << A[i][1] << std::endl;
		std::cout << A[i][2] << std::endl;
	}
	*/
	//	double *t = arr;
	//	Eigen::Matrix3d eigen_ = Eigen::MapType<Eigen::Matrix3d>( arr, 3, 3 );

	return O;
}

void MlfrsSimbody::init() {
	// setup contact
	contact.setTrackDissipatedEnergy(TRACK_DISSIPATED_ENERGY);
    contact.setTransitionVelocity(TRANSITION_VELOCITY);
	// setup the ground
	matter.Ground().updBody().addContactSurface(
		Transform(rXdown, Vec3(0)),
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
	//	state = simbody_sys.realizeTopology();
	simbody_sys.realizeTopology();
	//	state = system.getDefaultState();
	} catch  (const std::exception& e) {
		std::cout << "Error in realize() : " << e.what() << std::endl;
		exit;
	}
}
/*
 * Run the simbody simulation.
 */
void MlfrsSimbody::run() {
	try {
		/*
		 * Initialize, visualize and realize the simulation.
		 * Simbody requires this sequence(integrator/state) 
		 * before we can query the simulation.
		 */
		init();
		visualize();
		realize();

		// Here we iterate over all simbody objects and apply any extra runtime parameters.
		for(std::vector<simbodyObject>::iterator it = simbodyObjects.begin(); it != simbodyObjects.end(); ++it) {
			if ((*it).type == "pin") {
				MobilizedBody::Pin m = MobilizedBody::Pin::downcast((*it).mBody);
				m.setRate(state, 5.0);
			}
		}

		// integ.setMaximumStepSize(1e-0001);
		// integ.setAccuracy(.01);
		integ.setAccuracy(1e-3); // minimum for CPodes
		state = simbody_sys.getDefaultState();

	    TimeStepper ts(simbody_sys, integ);
	    ts.initialize(state);
		ts.stepTo(5.0);

		return;

	} catch  (const std::exception& e) {
		exit;
	}
}

MlfrsSimbody::~MlfrsSimbody() {}

