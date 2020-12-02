#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "Scene2.h"
#include "Camera.h"
#include "DemoObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MMath.h"
#include "Debug.h"
#include "Physics.h"
#include "Skybox.h"

Scene2::Scene2() : camera(nullptr), earth(nullptr), moon(nullptr), meshPtr(nullptr), shaderPtr(nullptr) {
	Debug::Info("Created Scene2: ", __FILE__, __LINE__);
}

Scene2::~Scene2() {}

bool Scene2::OnCreate() {
	camera = new Camera();

	skybox = new Skybox(camera, "textures/Stars/StarSkyboxPosy.png", "textures/Stars/StarSkyboxNegY.png", "textures/Stars/StarSkyboxNegx.png", "textures/Stars/StarSkyboxPosx.png", "textures/Stars/StarSkyboxnegz.png", "textures/Stars/StarSkyboxPosz.png");

	lightSource = Vec3(10.0f, 0.0f, -10.0f);
	GlobalTime = 0.0f;
	
	
	if (ObjLoader::loadOBJ("meshes/Sphere.obj") == false) {
		return false;
	}
	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderPtr = new Shader("basicTextureVert.glsl", "basicTextureFrag.glsl");
	earthTex = new Texture();
	moonTex = new Texture();
	if (meshPtr == nullptr || shaderPtr == nullptr || earthTex == nullptr || moonTex == nullptr) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}


	if (earthTex->LoadImage("textures/earthclouds.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}

	earth = new DemoObject(meshPtr, shaderPtr, earthTex);
	if (earth == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}

	if (moonTex->LoadImage("textures/moon.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}

	moon = new DemoObject(meshPtr, shaderPtr, moonTex);
	if (earth == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}

	earth->setPos(Vec3(0.0, 0.0, -10.0));
	moon->setPos(Vec3(-5.0, 0.0, 0.0));
	earth->setModelMatrix(MMath::translate(earth->getPos()));
	moon->setModelMatrix(MMath::translate(moon->getPos()));

	return true;
}

void Scene2::HandleEvents(const SDL_Event& sdlEvent) {
	camera->HandleEvents(sdlEvent);
}

void Scene2::Update(const float deltaTime) {
	earth->Update(deltaTime);
	static float rotation = 0.0f;
	rotation += 1.5f;
	moon->Update(deltaTime);

	earth->setModelMatrix(MMath::translate(earth->getPos()) * MMath::rotate(-rotation, Vec3(0.0f, 1.0f, 0.0f)) * MMath::rotate(-90.0f, Vec3(1.0f, 0.0f, 0.0f)));

	moon->setModelMatrix(MMath::translate(Vec3(0.0, 0.0,-10.0)) * MMath::rotate(-rotation, Vec3(0.0f, 1.0f, 0.0f)) * MMath::scale(Vec3(0.5f)) * earth->getModelMatrix());

	//camera->setViewMatrix(MMath::lookAt(MMath::rotate(rotation, Vec3(0.0f, 1.0f, 0.0f)) * Vec3(0.0f, 0.0f, 10.0f), Vec3(0.0f), Vec3(0.0f, 1.0f, 0.0f)));

	GlobalTime += deltaTime;
}

void Scene2::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	/// Draw your scene here
	
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	glDepthMask(GL_FALSE);
	
	glUseProgram(skybox->getShader()->getProgram());
	skybox->Skybox::Render();

	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	

	GLuint program = earth->getShader()->getProgram();
	glUseProgram(program);

	/// These pass the matricies and the light position to the GPU
	glUniformMatrix4fv(earth->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(earth->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix());
	glUniform3fv(earth->getShader()->getUniformID("lightPos"), 1, lightSource);
	glUniform1f(earth->getShader()->getUniformID("time"), GlobalTime);

	earth->Render();

	glUseProgram(moon->getShader()->getProgram());

	glUniformMatrix4fv(moon->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(moon->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix());
	glUniform3fv(moon->getShader()->getUniformID("lightPos"), 1, lightSource);
	glUniform1f(moon->getShader()->getUniformID("time"), GlobalTime);

	moon->Render();

	glUseProgram(0);
}

void Scene2::OnDestroy() {
	if (camera) delete camera, camera = nullptr;
	if (meshPtr) delete meshPtr, meshPtr = nullptr;
	if (earthTex) delete earthTex, earthTex = nullptr;
	if (moonTex) delete moonTex, moonTex = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (earth) delete earth, earth = nullptr;
	if (moon) delete moon, moon = nullptr;
	if (skybox) delete skybox, skybox = nullptr;
}