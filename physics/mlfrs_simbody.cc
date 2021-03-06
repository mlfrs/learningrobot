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
#include "simbody_input_handler.h"
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
const float STEP_SIZE = 1000.01;
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

ContactMaterial materialB(0.02*1e7,	// stiffness
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

void MlfrsSimbody::createSphere(MdlParser::mdl_object object, ObjectNode objectNode) {
	std::cout << "MlfrsSimbody::createSphere - object : " << objectNode.oId << std::endl;
	std::cout << "MlfrsSimbody::createSphere - object : " << objectNode.name << std::endl;
	try {
	    Body::Rigid bodyInfo(MassProperties(objectNode.mass, Vec3(0), objectNode.mass * UnitInertia::brick(
			Vec3(objectNode.dimensions[0]))));
		bodyInfo.addDecoration(Transform(), 
			DecorativeSphere(objectNode.dimensions[0]).setOpacity(0.4));
		bodyInfo.addContactSurface(Transform(),
				ContactSurface(ContactGeometry::Sphere(objectNode.dimensions[0]), material, 0.5)
		);
		rigidBody rbo = { bodyInfo, objectNode.oId };
	 	rigidBodies.push_back(rbo);
	} catch (const std::exception& e) {
		std::cout << "Error in createSphere() : " << e.what() << std::endl;
		exit;
	}
}

void MlfrsSimbody::createCylinder(MdlParser::mdl_object object, ObjectNode objectNode) {
	std::cout << "MlfrsSimbody::createCylinder - object : " << objectNode.oId << std::endl;
	std::cout << "MlfrsSimbody::createCylinder - object : " << objectNode.name << std::endl;
	try {
		ContactGeometry::TriangleMesh cylinderMesh(PolygonalMesh::createCylinderMesh(YAxis,
			objectNode.dimensions[0],objectNode.dimensions[1],10));

	    Body::Rigid bodyInfo(MassProperties( objectNode.mass, Vec3(0),
			objectNode.mass * UnitInertia::brick(
				Vec3(objectNode.dimensions[0], objectNode.dimensions[1], objectNode.dimensions[2]))));
		bodyInfo.addDecoration(Transform(), DecorativeCylinder(
			objectNode.dimensions[0], objectNode.dimensions[1]).setOpacity(0.4));
		// there is no primative cylinder... it's possible to use TriangleMesh geometry instead..
		// use brick instead
		bodyInfo.addContactSurface(Transform(),
			ContactSurface(cylinderMesh, materialB, 0.5));
//			ContactSurface(ContactGeometry::Brick(Vec3(
//				objectNode.dimensions[0], objectNode.dimensions[1], objectNode.dimensions[2])), materialB, 0.5));
		rigidBody rbo = { bodyInfo, objectNode.oId };
	 	rigidBodies.push_back(rbo);
	} catch (const std::exception& e) {
		std::cout << "Error in createCylinder() : " << e.what() << std::endl;
		exit;
	}
}

void MlfrsSimbody::createBox(MdlParser::mdl_object object, ObjectNode objectNode) {
	std::cout << "MlfrsSimbody::createBox - object : " << objectNode.oId << std::endl;
	std::cout << "MlfrsSimbody::createBox - object : " << objectNode.name << std::endl;
	try {

//		ContactGeometry::TriangleMesh brickMesh(PolygonalMesh::createBrickMesh(
//			Vec3(objectNode.dimensions[0],objectNode.dimensions[1],objectNode.dimensions[2]),10));

	    Body::Rigid bodyInfo(MassProperties(objectNode.mass, Vec3(0), objectNode.mass * UnitInertia::brick(
			Vec3(objectNode.dimensions[0],objectNode.dimensions[1],objectNode.dimensions[2]))));
		bodyInfo.addDecoration(Transform(), DecorativeBrick(
				Vec3(objectNode.dimensions[0], objectNode.dimensions[1], objectNode.dimensions[2]))
			.setOpacity(0.4));
		bodyInfo.addContactSurface(Transform(),
//				ContactSurface(brickMesh, materialB, 0.5));
				ContactSurface(ContactGeometry::Brick(
					Vec3(objectNode.dimensions[0], objectNode.dimensions[1], objectNode.dimensions[2])),
					material, 0.5));
		rigidBody rbo = { bodyInfo, objectNode.oId };
	 	rigidBodies.push_back(rbo);

	} catch (const std::exception& e) {
		std::cout << "Error in createBox() : " << e.what() << std::endl;
		exit;
	}
}

void MlfrsSimbody::createEllipsoid(MdlParser::mdl_object object, ObjectNode objectNode) {
	std::cout << "MlfrsSimbody::createBox - object : " << objectNode.oId << std::endl;
	std::cout << "MlfrsSimbody::createBox - object : " << objectNode.name << std::endl;
	try {

	    Body::Rigid bodyInfo(MassProperties(objectNode.mass, Vec3(0), objectNode.mass * UnitInertia::brick(
			Vec3(objectNode.dimensions[0],objectNode.dimensions[1],objectNode.dimensions[2]))));
		bodyInfo.addDecoration(Transform(), DecorativeEllipsoid(
				Vec3(objectNode.dimensions[0], objectNode.dimensions[1], objectNode.dimensions[2]))
			.setOpacity(0.4));
		bodyInfo.addContactSurface(Transform(),
//				ContactSurface(brickMesh, materialB, 0.5));
				ContactSurface(ContactGeometry::Ellipsoid(
					Vec3(objectNode.dimensions[0], objectNode.dimensions[1], objectNode.dimensions[2])),
					material, 0.5));
		rigidBody rbo = { bodyInfo, objectNode.oId };
	 	rigidBodies.push_back(rbo);

	} catch (const std::exception& e) {
		std::cout << "Error in createBox() : " << e.what() << std::endl;
		exit;
	}
}

/*
 * We need other types of contraints but this is the general idea.
 */
void MlfrsSimbody::createFreeJoint(MdlParser::mdl_joint joint, ObjectNode objectNode) {
	Body::Rigid rigidBody_;
	std::cout << "MlfrsSimbody::createFreeJoint : " << objectNode.name << std::endl;
	for(std::vector<rigidBody>::iterator it = rigidBodies.begin();
		it != rigidBodies.end(); ++it) {
		if ((*it).id == objectNode.oId) {
			rigidBody_ = (*it).rBody;
		}
	}
	SimTK::MobilizedBody::Free physicsBody(matter.Ground(), Transform( 
		Vec3(objectNode.position[0], objectNode.position[1], objectNode.position[2])), 
		rigidBody_, Transform(
		rYdown, 
		Vec3(joint.primary_position[0],
			 joint.primary_position[1],
			 joint.primary_position[2])));
	simbodyObject sbo = { physicsBody, "free", objectNode.oId, 0 };
 	simbodyObjects.push_back(sbo);
}

SimTK::MobilizedBody::Free MlfrsSimbody::createDefaultBody(MdlParser::mdl_joint joint, ObjectNode objectNode) {
	Body::Rigid rigidBody_;
	std::cout << "MlfrsSimbody::createFreeJoint : " << objectNode.name << std::endl;
	for(std::vector<rigidBody>::iterator it = rigidBodies.begin();
		it != rigidBodies.end(); ++it) {
		if ((*it).id == objectNode.oId) {
			rigidBody_ = (*it).rBody;
		}
	}
	SimTK::MobilizedBody::Free physicsBody(matter.Ground(), Transform( 
		Vec3(objectNode.position[0], objectNode.position[1], objectNode.position[2])), 
		rigidBody_, Transform(
		rYdown, 
		Vec3(joint.primary_position[0],
			 joint.primary_position[1],
			 joint.primary_position[2])));
	simbodyObject sbo = { physicsBody, "free", objectNode.oId, 0 };
 	simbodyObjects.push_back(sbo);
	return physicsBody;
}

/*
 * For a simbody joint and an object node.
 * The primary object (A) is the SimTK::MobilizedBody
 * The secondard object (B) is the Body::Rigid
 */
void MlfrsSimbody::createBallJoint(MdlParser::mdl_joint joint, ObjectNode objectNodeA, ObjectNode objectNodeB) {
	std::cout << "createBallJoint objectNodeA.name " << objectNodeA.name << std::endl;
	std::cout << "createBallJoint objectNodeB.name " << objectNodeB.name << std::endl;
	SimTK::MobilizedBody mobilizedBody;
	for(std::vector<simbodyObject>::iterator it = simbodyObjects.begin();
		it != simbodyObjects.end(); ++it) {
		if ((*it).rigidBodyAid == objectNodeA.oId) {
			mobilizedBody = (*it).mBody;
			std::cout << "objectNodeA.oId " << objectNodeA.oId << std::endl;
		}
	}
	if (!mobilizedBody.isInSubsystem()) {
		std::cout << "Does not exist - create free" << std::endl;
		mobilizedBody = createDefaultBody(joint, objectNodeA);
	}
	Body::Rigid bodyInfo;
	for(std::vector<rigidBody>::iterator it = rigidBodies.begin();
		it != rigidBodies.end(); ++it) {
		if ((*it).id == objectNodeB.oId) {
			bodyInfo = (*it).rBody;
			std::cout << "objectNodeB.oId " << objectNodeB.oId << std::endl;
		}
	}
	SimTK::MobilizedBody::Ball physicsBody(mobilizedBody, Transform(
		Rotation(
			Quaternion(
				Vec4(objectNodeA.rotation[0],
					 objectNodeA.rotation[1],
					 objectNodeA.rotation[2],
					 objectNodeA.rotation[3]))),
		Vec3(joint.primary_position[0],
			 joint.primary_position[1],
			 joint.primary_position[2])),
		bodyInfo, Transform(
		Rotation(
			Quaternion(
				Vec4(objectNodeB.rotation[0],
					 objectNodeB.rotation[1],
					 objectNodeB.rotation[2],
					 objectNodeB.rotation[3]))),
		Vec3(joint.secondary_position[0],
			 joint.secondary_position[1],
			 joint.secondary_position[2])));
	simbodyObject sbo = { physicsBody, "ball", objectNodeB.oId, 0 };
 	simbodyObjects.push_back(sbo);
	std::cout << " - - - -  " << std::endl;
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

void MlfrsSimbody::inputHandler() {
	simbody_sys.addEventHandler(new SimbodyInputHandler(*silo, Real(0.1))); 
}

void MlfrsSimbody::visualize() {
	try {
		silo = new Visualizer::InputSilo();
		Visualizer viz(simbody_sys);
		viz.setMode(Visualizer::RealTime);
		viz.setDesiredBufferLengthInSec(1);
		viz.setDesiredFrameRate(FRAME_RATE);
		viz.setGroundHeight(0);
		viz.setShowShadows(true);
		viz.addInputListener(silo);
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
		inputHandler();
		realize();
//		silo->waitForAnyUserInput(); 
//		silo->clear();

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
	silo->clear();
	timestep.stepTo(INTERVAL+=STEP_SIZE);
}

MlfrsSimbody::~MlfrsSimbody() {
	std::cout << " MlfrsSimbody::~MlfrsSimbody() " << std::endl; 
}

