#define HOST_ENDIAN_IS_BIG
#define BIG_ENDIAN

#include "Common/HardConfig.hpp"

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <thread>

#include "Common/CrossPlatform.hpp"

#include <RakNet/RakSleep.h>

#include "Server/MasterServer.hpp"
#include "Server/AuthServer.hpp"
#include "Server/WorldServer.hpp"
#include "Server/Bridges/BridgeMasterServer.hpp"
#include "API/API.hpp"
#include "GameCache/Interface/FastDatabase.hpp"
#include "DataTypes/AMF3.hpp"

#include "Utils/FileUtils.hpp"
#include "Utils/ServerInfo.hpp"
#include "Utils/StringUtils.hpp"
#include <chrono>

#include "Configuration/ConfigurationManager.hpp"
#include "Database/Database.hpp"
//#include "Server/UGCServer.hpp"

using namespace std::chrono;

std::vector<ILUServer *> virtualServerInstances;

enum class SERVERMODE : uint8_t { STANDALONE, MASTER, WORLD, AUTH, UGCOP } MODE_SERVER;

GameCache::Interface::FDB::Connection Cache;
BridgeMasterServer* masterServerBridge;

int givenWorldID = 2000;


// Following Includes are for testing
#include "FileTypes/LUZFile/LUZone.hpp"
#include "Entity/GameObject.hpp"
#include "GameCache/WorldConfig.hpp"
#include "Entity/Components/StatsComponent.hpp"
#include "DataTypes/LDF.hpp"
#include "FileTypes/HKXFile/hkxFile.hpp"
#include "Database/CacheImporter.hpp"

#include "Server/Manager/WorldInstanceManager.hpp"

#include <iostream>
#include <SDL/include/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "camera.h"
#include <vector>
#include <bullet3-2.89/src/btBulletDynamicsCommon.h> //<bullet/btBulletDynamicsCommon.h>	//you may need to change this

//#include <glad/include/glad/glad.h>
//#include <GLFW/glfw3.h>

camera cam;
GLUquadricObj* quad;
std::vector<btRigidBody*> bodies;
WorldServer* viewWs;
AuthServer* authServer = nullptr;


btRigidBody* addSphere(WorldServer* ws, float rad, float x, float y, float z, float mass)
{
	btTransform t;	//position and rotation
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));	//put it to x,y,z coordinates
	btSphereShape* sphere = new btSphereShape(rad);	//it's a sphere, so use sphereshape
	btVector3 inertia(0, 0, 0);	//inertia is 0,0,0 for static object, else
	if (mass != 0.0)
		sphere->calculateLocalInertia(mass, inertia);	//it can be determined by this function (for all kind of shapes)

	btMotionState* motion = new btDefaultMotionState(t);	//set the position (and motion)
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere, inertia);	//create the constructioninfo, you can create multiple bodies with the same info
	btRigidBody* body = new btRigidBody(info);	//let's create the body itself
	ws->dynamicsWorld->addRigidBody(body);	//and let the world know about it
	bodies.push_back(body);	//to be easier to clean, I store them a vector
	return body;
}

std::float_t colorFactor = 1.f / 255.f;

void renderSphere(btRigidBody* sphere, DataTypes::Vector3 color)
{
	if (sphere->getCollisionShape()->getShapeType() != SPHERE_SHAPE_PROXYTYPE)	//only render, if it's a sphere
		return;
	glColor3f(colorFactor * color.x, colorFactor * color.y, colorFactor * color.z);
	float r = ((btSphereShape*)sphere->getCollisionShape())->getRadius();
	btTransform t;
	sphere->getMotionState()->getWorldTransform(t);	//get the transform
	t = sphere->getWorldTransform();
	float mat[16];
	t.getOpenGLMatrix(mat);	//OpenGL matrix stores the rotation and orientation
	glPushMatrix();
	glMultMatrixf(mat);	//multiplying the current matrix with it moves the object in place
	gluSphere(quad, r, 20, 20);
	glPopMatrix();
}

//similar then renderSphere function
void renderPlane(btRigidBody* plane)
{
	if (plane->getCollisionShape()->getShapeType() != STATIC_PLANE_PROXYTYPE)
		return;
	glColor3f(0.8, 0.8, 0.8);
	btTransform t;
	plane->getMotionState()->getWorldTransform(t);
	float mat[16];
	t.getOpenGLMatrix(mat);
	glPushMatrix();
	glMultMatrixf(mat);	//translation,rotation
	glBegin(GL_QUADS);
	glVertex3f(-1000, 0, 1000);
	glVertex3f(-1000, 0, -1000);
	glVertex3f(1000, 0, -1000);
	glVertex3f(1000, 0, 1000);
	glEnd();
	glPopMatrix();
}

void renderCube(btRigidBody* rb)
{
	if (rb->getCollisionShape()->getShapeType() != BOX_SHAPE_PROXYTYPE)
		return;
	glColor3f(0, 0.8, 0);
	btTransform t;
	rb->getMotionState()->getWorldTransform(t);
	float mat[16];
	t.getOpenGLMatrix(mat);
	glPushMatrix();
	btVector3 sz = rb->getCollisionShape()->getLocalScaling();
	glMultMatrixf(mat);	//translation,rotation
	glBegin(GL_POLYGON);/* f1: front */
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glEnd();
	glBegin(GL_POLYGON);/* f2: bottom */
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glEnd();
	glBegin(GL_POLYGON);/* f3:back */
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glEnd();
	glBegin(GL_POLYGON);/* f4: top */
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 1.0f);
	glEnd();
	glBegin(GL_POLYGON);/* f5: left */
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();
	glBegin(GL_POLYGON);/* f6: right */
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glEnd();
	glPopMatrix();
}


void display(WorldServer * ws)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	cam.Control();
	//drawSkybox(50);
	cam.UpdateCamera();
	for (int i = 0; i < bodies.size(); i++)
	{
		if (bodies[i]->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE)
			renderPlane(bodies[i]);
		else if (bodies[i]->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE)
			renderSphere(bodies[i], DataTypes::Vector3(255, 0, 0));
	}

	ObjectsManager* objMan = ws->objectsManager;
	auto objects = objMan->GetObjects();
	for (auto it = objects.begin(); it != objects.end(); ++it) {
		btRigidBody* rb = (*it)->GetRigidBody();
		if (rb == nullptr) continue;
		if (rb->getCollisionShape()->getShapeType() == BroadphaseNativeTypes::BOX_SHAPE_PROXYTYPE) {
			renderCube(rb);
		}
		else if (rb->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE)
			renderPlane(rb);
		else if (rb->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE) {
			auto colBig = std::hash<std::uint32_t>()((*it)->GetLOT());
			renderSphere(rb,DataTypes::Vector3((colBig & 0xFF0000) >> 16, (colBig & 0x00FF00) >> 8, (colBig & 0x0000FF)));
			btTransform t = rb->getWorldTransform();
			//Logger::log("TEST", "Rendering sphere at " + std::to_string(t.getOrigin().x()) + " " + std::to_string(t.getOrigin().y()) + " " + std::to_string(t.getOrigin().z()));
		}
	}
}

void init(WorldServer * ws, float angle)
{
	//pretty much initialize everything logically
	/*ws->collisionConfiguration = new btDefaultCollisionConfiguration();
	ws->collisionDispatcher = new btCollisionDispatcher(collisionConfig);
	auto broadphase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
	world->setGravity(btVector3(0, -10, 0));	//gravity on Earth
	*/
	//similar to createSphere
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(0, 0, 0));
	btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
	btRigidBody* body = new btRigidBody(info);
	ws->dynamicsWorld->addRigidBody(body);
	bodies.push_back(body);

	//addSphere(ws, 1.0, 0, 20, 0, 1.0);	//add a new sphere above the ground 

	glClearColor(0, 0, 0, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angle, 640.0 / 480.0, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	quad = gluNewQuadric();
	//initskybox();
	glEnable(GL_DEPTH_TEST);
	cam.setLocation(vector3d(10, 10, 10));	//the player will be top of the terrain
}


void TestPhysics() {
	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER == SERVERMODE::MASTER) {
		std::thread mT([](MasterServer* ms) { ms = new MasterServer(); }, ServerInfo::masterServer);
		mT.detach();

		std::thread wiT([]() { StartAPI(); });
		wiT.detach();
	}

	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER != SERVERMODE::MASTER) {
		masterServerBridge = new BridgeMasterServer("127.0.0.1");
		masterServerBridge->Connect();
		masterServerBridge->Listen();
	}

	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER == SERVERMODE::AUTH) {
		AuthServer* aS;
		std::thread aT([](AuthServer* as) { as = new AuthServer(); }, aS);
		aT.detach();
	}
	/*
	WorldServer* testWs;
	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER == SERVERMODE::WORLD) {
		std::thread wT([](WorldServer* ws) { ws = new WorldServer(givenWorldID, 0, 2001); }, testWs);
		wT.detach();
	}
	*/
	ServerInfo::init();

	//while (virtualServerInstances.size() == 0) { Sleep(30); }
	//testWs = static_cast<WorldServer*>(virtualServerInstances.at(0));
	//viewWs = testWs;

	/*SDL_Init(SDL_INIT_EVERYTHING);
	SDL_SetVideoMode(1600, 900, 32, SDL_OPENGL);
	Uint32 start;
	SDL_Event event;
	bool running = true;
	float angle = 50;
	init(testWs, angle);
	while (running)
	{
		start = SDL_GetTicks();
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					running = false;
					break;
				case SDLK_y:
					cam.mouseIn(false);
					break;
				case SDLK_SPACE:
					//if space is pressed, shoot a ball
					btRigidBody* sphere = addSphere(testWs, 1.0, cam.getLocation().x, cam.getLocation().y, cam.getLocation().z, 1.0);
					vector3d look = cam.getVector() * 20;
					sphere->setLinearVelocity(btVector3(look.x, look.y, look.z));
					break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{

				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				cam.mouseIn(true);
				break;

			}
		}
		testWs->dynamicsWorld->stepSimulation(1 / 60.0);	//you have to call this function, to update the simulation (with the time since last time in seconds), you can actually call this with varying variables, so you can actually mease the time since last update
		display(testWs);
		SDL_GL_SwapBuffers();
		if (1000.0f / 60 > SDL_GetTicks() - start)
			SDL_Delay(1000.0f / 60 - (SDL_GetTicks() - start));
	}
	//killskybox();
	for (int i = 0; i < bodies.size(); i++)
	{
		testWs->dynamicsWorld->removeCollisionObject(bodies[i]);
		btMotionState* motionState = bodies[i]->getMotionState();
		btCollisionShape* shape = bodies[i]->getCollisionShape();
		delete bodies[i];
		delete shape;
		delete motionState;
	}
	SDL_Quit();
	gluDeleteQuadric(quad);*/

}

int main(int argc, char* argv[]) {
	FileUtils::ChangeDirectory();
	ServerInfo::init();

	std::string ipMaster = "127.0.0.1";
	//std::string ipMaster = "foxsog.com";

	std::string hf = "res/physics/env_nim_ag_puffytree.hkx";
	HKX::HKXFile hkx; hkx.Load(hf);

	Configuration::ConfigurationManager::Load();

	Logger::log("MAIN", "Connecting to database...");
	Database::Connect();

	Logger::log("MAIN", "Connecting game cache...");
	Cache.Connect("./res/cdclient.fdb");

	using namespace Entity;

	MODE_SERVER = SERVERMODE::STANDALONE;

	//TestPhysics(); return 0;

	// Logger::log("MAIN", "Setting up GLFW in case a renderer is being active.");
	/*glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

	Logger::log("MAIN", "Booting up server instances...");

	//CacheImporter::Import();

	//std::uintptr_t * c = CreateCOPScene();

#ifdef OPCRUX_PLATFORM_WIN32
	std::system("title OpCrux Server (Standalone)");
#endif
	for (std::ptrdiff_t i = 0; i < argc; i++) {
		std::string arg = std::string(argv[i]);
		if (arg == "--master") {
			MODE_SERVER = SERVERMODE::MASTER;
#ifdef OPCRUX_PLATFORM_WIN32
			std::system("title OpCrux Server (Master only)");
#endif
		}
		else if (arg == "--world") {
			MODE_SERVER = SERVERMODE::WORLD;
#ifdef OPCRUX_PLATFORM_WIN32
			std::system("title OpCrux Server (World only)");
#endif
		}
		else if (arg == "--auth") {
			MODE_SERVER = SERVERMODE::AUTH;
#ifdef OPCRUX_PLATFORM_WIN32
			std::system("title OpCrux Server (Auth only)");
#endif
		}
		else if (arg == "--ugcop") {
			MODE_SERVER = SERVERMODE::UGCOP;
#ifdef OPCRUX_PLATFORM_WIN32
			std::system("title OpCrux Server (UGC only)");
#endif
		}
		else if (arg == "--worldID" && i < argc) {
			givenWorldID = std::stoi(argv[i + 1]);
		}

		if (argc >= 4) {
			ipMaster = argv[i + 1];
		}
	}

	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER == SERVERMODE::MASTER) {
		std::thread mT([](MasterServer* ms) { ms = new MasterServer(); }, ServerInfo::masterServer);
		mT.detach();

		if (Configuration::ConfigurationManager::dbConf.GetStringVal("API", "USESSL") == "TRUE") {
			/*std::thread wiT([]() { StartAPIWithSSL(); });
			wiT.detach();*/
		}
		else {
			std::thread wiT([]() { StartAPI(); });
			wiT.detach();
		}
	}

	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER != SERVERMODE::MASTER) {
		masterServerBridge = new BridgeMasterServer(ipMaster);
		masterServerBridge->Connect();
		masterServerBridge->Listen();
	}

	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER == SERVERMODE::AUTH) {
		std::thread aT([](AuthServer ** as) { new AuthServer(); }, &authServer);
		aT.detach();
	}

	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER == SERVERMODE::WORLD) {
		//WorldServer * charSelectWs;
		//std::thread wT([](WorldServer * ws) { ws = new WorldServer(givenWorldID, 0,2001); }, charSelectWs);
		//wT.detach();
	
		//WorldInstanceManager::CreateInstance(1000, 0, 0, 2100);
	}

	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER == SERVERMODE::UGCOP) {
		//UGCServer* ugcServer;
		//std::thread ugcT([](UGCServer* ugcs) {ugcs = new UGCServer(); }, ugcServer);
		//ugcT.detach();
	}

	while (ServerInfo::bRunning) {RakSleep(30);}
	
	return 0;
}
