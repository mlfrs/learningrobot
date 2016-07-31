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
float INTERVAL = 0;
const float STEP_SIZE = 0.01;
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
								integ(simbody_sys,CPodes::BDF,CPodes::Newton),
								timestep(simbody_sys, integ)
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
		rigidBody rbo = { bodyInfo, objectNode.oId };
	 	rigidBodies.push_back(rbo);
		//.joinClique(defaultClique));
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
		rigidBody rbo = { bodyInfo, objectNode.oId };
	 	rigidBodies.push_back(rbo);
		//.joinClique(defaultClique));
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
		rigidBody rbo = { bodyInfo, objectNode.oId };
	 	rigidBodies.push_back(rbo);
	} catch (const std::exception& e) {
		std::cout << "Error in createCylinder() : " << e.what() << std::endl;
		exit;
	}
}

/*
 * We need other types of contraints but this is the general idea.
 * For this use case just say a free joint is always connected with ground
 */
void MlfrsSimbody::createFreeJoint(MdlParser::mdl_joint joint, ObjectNode objectNodeA, ObjectNode objectNodeB) {
	Body::Rigid rigidBodyA, rigidBodyB;
	for(std::vector<rigidBody>::iterator it = rigidBodies.begin();
		it != rigidBodies.end(); ++it) {
		if ((*it).id == objectNodeA.oId) {
			rigidBodyA = (*it).rBody;
		}
		if ((*it).id == objectNodeB.oId) {
			rigidBodyB = (*it).rBody;
		}
	}
	SimTK::MobilizedBody::Free physicsBody(matter.Ground(), Transform(Vec3(0)),
		rigidBodyA, Transform( rYdown, 
		Vec3(joint.primary_position[0],joint.primary_position[1],joint.primary_position[2])));
	simbodyObject sbo = { physicsBody, "free", objectNodeA.oId, 0 };
 	simbodyObjects.push_back(sbo);
}

/*
 * For a simbody joint and an object node.
 * The primary object (A) is the SimTK::MobilizedBody
 * The secondard object (B) is the Body::Rigid
 */
void MlfrsSimbody::createBallJoint(MdlParser::mdl_joint joint, ObjectNode objectNodeA, ObjectNode objectNodeB) {
	SimTK::MobilizedBody mobilizedBody;
	for(std::vector<simbodyObject>::iterator it = simbodyObjects.begin();
		it != simbodyObjects.end(); ++it) {
		if ((*it).rigidBodyAid == objectNodeA.oId) {
			mobilizedBody = (*it).mBody;
		}
	}
	Body::Rigid bodyInfo;
	for(std::vector<rigidBody>::iterator it = rigidBodies.begin();
		it != rigidBodies.end(); ++it) {
		if ((*it).id == objectNodeB.oId) {
			bodyInfo = (*it).rBody;
		}
	}
	SimTK::MobilizedBody::Ball physicsBody(mobilizedBody, Transform(
		Vec3(joint.primary_position[0],joint.primary_position[1],joint.primary_position[2])),
		bodyInfo, Transform( rYdown, 
		Vec3(joint.primary_position[0],joint.primary_position[1],joint.primary_position[2])));
	simbodyObject sbo = { physicsBody, "free", objectNodeB.oId, 0 };
 	simbodyObjects.push_back(sbo);
}

/*
 * Takes an objectNode and returns a copy if it with the current state.
 */
ObjectNode MlfrsSimbody::getStatefulObjectNode(ObjectNode node) {
	for(std::vector<simbodyObject>::iterator it = simbodyObjects.begin();
		it != simbodyObjects.end(); ++it) {
		if ((*it).rigidBodyAid == node.oId) {
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
	// const SimTK::State &state_ = getAssembler().getInternalState();

	const Vec3 P = mBody.getBodyOriginLocation(state_);
	const Rotation R = mBody.getBodyRotation(state_);
	const Vec3 LV = mBody.getBodyOriginVelocity(state_);
	const Vec3 AV = mBody.getBodyAngularVelocity(state_);
	const Vec3 LA = mBody.getBodyOriginAcceleration(state_);
	const Vec3 AA = mBody.getBodyAngularAcceleration(state_);
	const Vec4 AR = R.convertRotationToAngleAxis();

	O.position=Eigen::Vector3f(P[0],P[1],P[2]);
	O.rotation=Eigen::Vector4f(AR[0],AR[1],AR[2],AR[3]);
	O.linear_velocity=Eigen::Vector3f(LV[0],LV[1],LV[2]);
	O.angular_velocity=Eigen::Vector3f(AV[0],AV[1],AV[2]);
	O.linear_acceleration=Eigen::Vector3f(LA[0],LA[1],LA[2]);
	O.angular_acceleration=Eigen::Vector3f(AA[0],AA[1],AA[2]);

	// std::cout << " Eigen::Vector4f - " << O.position << std::endl;
	// std::cout << " Eigen::Vector4f - " << O.rotation << std::endl;

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

void MlfrsSimbody::setup() {
	// setup contact
	contact.setTrackDissipatedEnergy(TRACK_DISSIPATED_ENERGY);
    contact.setTransitionVelocity(TRANSITION_VELOCITY);
	// setup the ground
	matter.Ground().updBody().addContactSurface(
		Transform(rXdown, Vec3(0)),
		ContactSurface(ContactGeometry::HalfSpace(), material));
	// .joinClique(ContactSurface::createNewContactClique()));
}

void MlfrsSimbody::printContactSurfaces() {
	for (MobilizedBodyIndex mbx(0); mbx < matter.getNumBodies(); ++mbx) {
		const MobilizedBody& mBody = matter.getMobilizedBody(mbx);
		const int nsurfs = mBody.getBody().getNumContactSurfaces();
		std::cout << "Movable body " << (int)mbx << " has " << nsurfs << " contact surfaces" << std::endl;
		for (int i=0; i<nsurfs; ++i) {
		// std::cout << " " << i << " - index : " << (int)tracker.getContactSurfaceIndex(mbx,i) << std::endl;
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
 * initialize the simbody simulation.
 */
void MlfrsSimbody::init() {
	try {
		/*
		 * Initialize, visualize and realize the simulation.
		 * Simbody requires this sequence(integrator/state) 
		 * before we can query the simulation.
		 */
		setup();
		visualize();
		realize();

		// Here we iterate over all simbody objects and apply any extra runtime parameters.
		for(std::vector<simbodyObject>::iterator it = simbodyObjects.begin(); it != simbodyObjects.end(); ++it) {
			if ((*it).type == "pin") {
				MobilizedBody::Pin m = MobilizedBody::Pin::downcast((*it).mBody);
				m.setRate(state, 10.0);
			}
		}

		// integ.setMaximumStepSize(1e-0001);
		// integ.setAccuracy(.01);
		integ.setAccuracy(1e-3); // minimum for CPodes
		state = simbody_sys.getDefaultState();

		// TimeStepper ts(simbody_sys, integ);
	    timestep.initialize(state);

	} catch  (const std::exception& e) {
		exit;
	}
}

/*
 * Step the simulation.
 */
void MlfrsSimbody::run() {
	timestep.stepTo(INTERVAL+=STEP_SIZE);
}

MlfrsSimbody::~MlfrsSimbody() {
	std::cout << " MlfrsSimbody::~MlfrsSimbody() " << std::endl; 
}

