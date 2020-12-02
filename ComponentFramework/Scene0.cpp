#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "Scene0.h"
#include "Camera.h"
#include "DemoObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MMath.h"
#include "Debug.h"
#include "Physics.h"


Scene0::Scene0(): camera(nullptr), demoObject(nullptr),meshPtr(nullptr),shaderPtr(nullptr),texturePtr(nullptr) {
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
	glEnable(GL_PROGRAM_POINT_SIZE);
}

Scene0::~Scene0() {}

bool Scene0::OnCreate() {
	camera = new Camera();
	lightSource = Vec3(5.0f, 10.0f, -5.0f);
	GlobalTime = 0.0f;
	if (ObjLoader::loadOBJ("meshes/Skull.obj") == false) {
		return false;
	}
	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderPtr = new Shader("textureVert.glsl", "textureFrag.glsl");
	texturePtr = new Texture();
	if (meshPtr == nullptr|| shaderPtr == nullptr || texturePtr == nullptr) {
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

void Scene0::HandleEvents(const SDL_Event &sdlEvent) {
	camera->HandleEvents(sdlEvent);
}

void Scene0::Update(const float deltaTime) {
	demoObject->Update(deltaTime); 
	static float rotation = 0.0f;
	rotation += 1.5f;
	
	//demoObject->setVel(Vec3(1.0, 0.0, 0.0));
	//Physics::SimpleNewtonMotion(*demoObject, deltaTime);
	demoObject->setModelMatrix(MMath::rotate(rotation, Vec3(0.0f, 1.0f, 0.0f)));
	//demoObject->setModelMatrix(MMath::translate(demoObject->getPos()));
	GlobalTime += deltaTime;
}

void Scene0::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	/// Draw your scene here
	GLuint program = demoObject->getShader()->getProgram();
	glUseProgram(program);

	/// These pass the matricies and the light position to the GPU
	glUniformMatrix4fv(demoObject->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(demoObject->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix());
	glUniform3fv(demoObject->getShader()->getUniformID("lightPos"), 1, lightSource);
	glUniform1f(demoObject->getShader()->getUniformID("time"), GlobalTime);

	demoObject->Render();

	glUseProgram(0);
}


void Scene0::OnDestroy() {
	if (camera) delete camera, camera = nullptr;
	if (meshPtr) delete meshPtr, meshPtr = nullptr;
	if (texturePtr) delete texturePtr, texturePtr = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (demoObject) delete demoObject, demoObject = nullptr;
}
