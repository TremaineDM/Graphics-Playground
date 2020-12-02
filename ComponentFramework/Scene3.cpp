#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "Scene3.h"
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

Scene3::Scene3() : camera(nullptr), sphere(nullptr), meshPtr(nullptr), shaderPtr(nullptr) {
	Debug::Info("Created Scene3: ", __FILE__, __LINE__);
}

Scene3::~Scene3() {}

bool Scene3::OnCreate() {
	camera = new Camera();

	skybox = new Skybox(camera, "textures/CNTower/posy.jpg", "textures/CNTower/negy.jpg", "textures/CNTower/negx.jpg", "textures/CNTower/posx.jpg", "textures/CNTower/negz.jpg", "textures/CNTower/posz.jpg");

	//lightSource = Vec3(10.0f, 0.0f, -10.0f);
	GlobalTime = 0.0f;
	if (ObjLoader::loadOBJ("meshes/Sphere.obj") == false) {
		return false;
	}
	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderPtr = new Shader("colourSphereVert.glsl", "colourSphereFrag.glsl");

	if (meshPtr == nullptr || shaderPtr == nullptr) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}



	sphere = new DemoObject(meshPtr, shaderPtr, nullptr);
	if (sphere == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}

	noiseTex = CreateNoise3D();

	sphere->setPos(Vec3(0.0, 0.0, 0.0));

	sphere->setModelMatrix(MMath::translate(sphere->getPos()) * MMath::scale(Vec3(3.0f)));

	return true;
}

void Scene3::HandleEvents(const SDL_Event& sdlEvent) {
	camera->HandleEvents(sdlEvent);
}

void Scene3::Update(const float deltaTime) {
	sphere->Update(deltaTime);

	GlobalTime += deltaTime;
}

void Scene3::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	/// Draw your scene here

	glDisable(GL_CULL_FACE);

	glDepthMask(GL_FALSE);

	glUseProgram(skybox->getShader()->getProgram());
	skybox->Skybox::Render();

	glDepthMask(GL_TRUE);



	GLuint program = sphere->getShader()->getProgram();
	glUseProgram(program);

	/// These pass the matricies and the light position to the GPU
	glUniformMatrix4fv(sphere->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(sphere->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix());
	glUniform1f(sphere->getShader()->getUniformID("time"), GlobalTime);
	glBindTexture(GL_TEXTURE_3D, noiseTex);

	sphere->Render();

	glUseProgram(0);
}


void Scene3::OnDestroy() {
	if (camera) delete camera, camera = nullptr;
	if (meshPtr) delete meshPtr, meshPtr = nullptr;
	if (sphere) delete sphere, sphere = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (skybox) delete skybox, skybox = nullptr;
}
