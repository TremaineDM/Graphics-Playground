#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "Scene1.h"
#include "Camera.h"
#include "DemoObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MMath.h"
#include "Debug.h"
#include "Physics.h"

Scene1::Scene1() : camera(nullptr), demoObject(nullptr), meshPtr(nullptr), shaderPtr(nullptr), texturePtr(nullptr) {
	Debug::Info("Created Scene1: ", __FILE__, __LINE__);
}

Scene1::~Scene1() {}

bool Scene1::OnCreate() {
	camera = new Camera();
	lightSource[0] = Vec3(15.0f, 15.0f, -10.0f);
	lightSource[1] = Vec3(-15.0f, 15.0f, -10.0f);
	lightSource[2] = Vec3(0.0f, -10.0f, -10.0f);


	GlobalTime = 0.0f;
	if (ObjLoader::loadOBJ("meshes/Skull.obj") == false) {
		return false;
	}
	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderPtr = new Shader("multiPhongVert.glsl", "multiPhongFrag.glsl");
	texturePtr = new Texture();
	if (meshPtr == nullptr || shaderPtr == nullptr || texturePtr == nullptr) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}


	if (texturePtr->LoadImage("textures/skull_texture.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}

	demoObject = new DemoObject(meshPtr, shaderPtr, texturePtr);
	if (demoObject == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	demoObject->setPos(Vec3(0.0, 0.0, 0.0));
	demoObject->setModelMatrix(MMath::translate(demoObject->getPos()));

	return true;
}

void Scene1::HandleEvents(const SDL_Event& sdlEvent) {
	camera->HandleEvents(sdlEvent);
}

void Scene1::Update(const float deltaTime) {
	demoObject->Update(deltaTime);
	static float rotation = 0.0f;
	rotation += 1.5f;

	//demoObject->setVel(Vec3(1.0, 0.0, 0.0));
	//Physics::SimpleNewtonMotion(*demoObject, deltaTime);
	demoObject->setModelMatrix(MMath::rotate(rotation, Vec3(0.0f, 1.0f, 0.0f)) * MMath::translate(Vec3(5,0,0)));
	//demoObject->setModelMatrix(MMath::translate(demoObject->getPos()));
	GlobalTime += deltaTime;
}

void Scene1::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	/// Draw your scene here
	GLuint program = demoObject->getShader()->getProgram();
	glUseProgram(program);

	/// These pass the matricies and the light position to the GPU
	glUniformMatrix4fv(demoObject->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(demoObject->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix());
	//glUniform1i(demoObject->getShader()->getUniformID("lightNumber"), 1);

	glUniform1f(demoObject->getShader()->getUniformID("time"), GlobalTime);
	
	glUniform3fv(demoObject->getShader()->getUniformID("lightPos[0]"), 3, *lightSource);
	

	demoObject->Render();

	glUseProgram(0);
}


void Scene1::OnDestroy() {
	if (camera) delete camera, camera = nullptr;
	if (meshPtr) delete meshPtr, meshPtr = nullptr;
	if (texturePtr) delete texturePtr, texturePtr = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (demoObject) delete demoObject, demoObject = nullptr;
}
