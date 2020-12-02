#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "Scene4.h"
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
#include "Noise.h"

Scene4::Scene4() : camera(nullptr), sphere(nullptr), meshPtr(nullptr), shaderPtr(nullptr) {
	Debug::Info("Created Scene4: ", __FILE__, __LINE__);
}

Scene4::~Scene4() {}

bool Scene4::OnCreate() {
	camera = new Camera();

	skybox = new Skybox(camera, "textures/CNTower/posy.jpg", "textures/CNTower/negy.jpg", "textures/CNTower/negx.jpg", "textures/CNTower/posx.jpg", "textures/CNTower/negz.jpg", "textures/CNTower/posz.jpg");

	lightSource = Vec3(-10.0f, 10.0f, 10.0f);
	GlobalTime = 0.0f;
	if (ObjLoader::loadOBJ("meshes/Skull.obj") == false) {
		return false;
	}
	meshPtr = new Mesh(GL_PATCHES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderPtr = new Shader("reflectionVert.glsl", "reflectionFrag.glsl", "reflectionTessC.glsl", "reflectionTessE.glsl", "reflectionGeo.glsl");

	if (meshPtr == nullptr || shaderPtr == nullptr) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}



	sphere = new DemoObject(meshPtr, shaderPtr, nullptr);
	if (sphere == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}

	noiseTexID = CreateNoise3D();

	sphere->setPos(Vec3(0.0, 0.0, 0.0));

	sphere->setModelMatrix(MMath::translate(sphere->getPos()) * MMath::scale(Vec3(1.5f)));

	return true;
}

void Scene4::HandleEvents(const SDL_Event& sdlEvent) {
	camera->HandleEvents(sdlEvent);
}

void Scene4::Update(const float deltaTime) {
	sphere->Update(deltaTime);
	//sphere->setModelMatrix(MMath::translate(-Vec3(0.0f,0.0f,(cos(GlobalTime) * 25.0f))) * MMath::scale(Vec3(1.5f)));
	GlobalTime += deltaTime;
}

void Scene4::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);

	glUseProgram(skybox->getShader()->getProgram());
	skybox->Skybox::Render();
	glUseProgram(0);

	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);

	GLuint program = sphere->getShader()->getProgram();
	glUseProgram(program);

	/// These pass the matricies and the light position to the GPU
	glUniformMatrix4fv(sphere->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(sphere->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix());
	glUniform3fv(sphere->getShader()->getUniformID("lightPos"), 1, lightSource);
	glUniform1f(sphere->getShader()->getUniformID("time"), GlobalTime);

	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getSkyboxTexture());
	glBindTexture(GL_TEXTURE_3D, noiseTexID);

	sphere->Render();

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glUseProgram(0);
}


void Scene4::OnDestroy() {
	if (camera) delete camera, camera = nullptr;
	if (meshPtr) delete meshPtr, meshPtr = nullptr;
	if (sphere) delete sphere, sphere = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (skybox) delete skybox, skybox = nullptr;
	DeleteNoise3D();
}
