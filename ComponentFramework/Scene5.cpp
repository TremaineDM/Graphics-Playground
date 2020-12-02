#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "Scene5.h"
#include "Camera.h"
#include "DemoObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Debug.h"
#include "Physics.h"
#include "Skybox.h"
#define SHADOWWIDTH 1024
#define SHADOWHEIGHT 1024

Scene5::Scene5() : camera(nullptr), sphere(nullptr), meshPtr(nullptr), shaderPtr(nullptr) {
	Debug::Info("Created Scene5: ", __FILE__, __LINE__);
}

Scene5::~Scene5() {}

bool Scene5::OnCreate() {
	camera = new Camera();

	//skybox = new Skybox(camera, "textures/CNTower/posy.jpg", "textures/CNTower/negy.jpg", "textures/CNTower/negx.jpg", "textures/CNTower/posx.jpg", "textures/CNTower/negz.jpg", "textures/CNTower/posz.jpg");

	lightSource = Vec3(0.0f, 3.0f, 20.0f);
	GlobalTime = 0.0f;
	if (ObjLoader::loadOBJ("meshes/Sphere.obj") == false) {
		return false;
	}
	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderPtr = new Shader("phongVert.glsl", "phongFrag.glsl");
	depthShader = new Shader("depthVert.glsl", "depthFrag.glsl");
	debugdepthShader = new Shader("debugdepthVert.glsl", "debugdepthFrag.glsl");
	
	//lightProj = MMath::perspective(90.0f, (16.0f / 9.0f), 0.1f, 50.0f);
	lightProj = MMath::orthographic(-30.0f, 30.0f, -30.0f, 30.0f, -10.0f, 50.0f);
	lightView = MMath::lookAt(lightSource, Vec3(0.0f), Vec3(0.0f, 1.0f, 0.0f));

	if (meshPtr == nullptr || shaderPtr == nullptr) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}

	sphere = new DemoObject(meshPtr, shaderPtr, nullptr);
	if (sphere == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}

	if (ObjLoader::loadOBJ("meshes/Cube.obj") == false) {
		return false;
	}

	cubeMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);

	botWall = new PlaneObject(cubeMesh, shaderPtr, nullptr);
	backWall = new PlaneObject(cubeMesh, shaderPtr, nullptr);
	leftWall = new PlaneObject(cubeMesh, shaderPtr, nullptr);

	sphere->setPos(Vec3(0.0, 0.0, 0.0));
	sphere->setModelMatrix(MMath::translate(sphere->getPos()) * MMath::scale(Vec3(1.0f)));

	botWall->plane = Plane(0.0f, 1.0f, 0.0f, 8.0f);
	backWall->plane = Plane(0.0f, 0.0f, 1.0f, 8.0f);
	leftWall->plane = Plane(1.0f, 0.0f, 0.0f, 8.0f);

	//Setup depth frame buffer and texture
	glGenFramebuffers(1, &depthMapBuffer);
	glGenTextures(1, &depthMapTexture);

	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWWIDTH, SHADOWHEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f,1.0f,1.0f,1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	lightView.print();
	camera->getViewMatrix().print();
	sphere->setAccel(Vec3(0.0f, -9.81f, 0.0f));
	return true;
}

void Scene5::HandleEvents(const SDL_Event& sdlEvent) 
{
	camera->HandleEvents(sdlEvent);
}

void Scene5::Update(const float deltaTime) 
{
	
	sphere->setPos(sphere->getPos() + sphere->getVel() * deltaTime + 0.5 * sphere->getAccel() * deltaTime * deltaTime);
	sphere->setVel(sphere->getVel() + sphere->getAccel() * deltaTime);
	if (sphere->getPos().y < -7.5f)
	{
		sphere->setVel(-sphere->getVel());
	}

	sphere->setModelMatrix(MMath::translate(sphere->getPos()));
	//lightSource = Vec3(sin(GlobalTime * 1.0f) * 2.0f + 0.0f, -5.0f, 20.0f);
	lightSource = Vec3((sin(GlobalTime * 0.5f) * 8.0f) - 10.0f, -5.0f, (cos(GlobalTime * 0.5f) * 8.0f) + 10.0f);
	
	lightView = MMath::lookAt(lightSource, Vec3(0.0f), VMath::cross(lightSource, Vec3(0.1,0.0,0.0)));

	botWall->Update(deltaTime);
	backWall->Update(deltaTime);
	leftWall->Update(deltaTime);

	GlobalTime += deltaTime;
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions			// texture Coords
			-1.0f, -0.5f, 0.0f,		0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,
			-0.5f, -0.5f, 0.0f,		1.0f, 1.0f,
			-0.5f, -1.0f, 0.0f,		1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void Scene5::RenderShadowTexture(DemoObject* obj) const
{
	glUseProgram(depthShader->getProgram());
	glUniformMatrix4fv(depthShader->getUniformID("lightSpaceMatrix"), 1, GL_FALSE, lightProj * lightView);
	glUniformMatrix4fv(depthShader->getUniformID("model"), 1, GL_FALSE, obj->getModelMatrix());
	obj->Render();

	glUseProgram(0);
}

void Scene5::Render() const {
	//First render to the depth map
	{
		glViewport(0, 0, SHADOWWIDTH, SHADOWHEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapBuffer);
		glClearColor(0.0f, 0.0f, 1.0f, 0.5f);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		RenderShadowTexture(sphere);
		RenderShadowTexture(botWall);
		RenderShadowTexture(backWall);
		RenderShadowTexture(leftWall);

		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	{
		glViewport(0, 0, 1280, 720);
		glClearColor(0.0f, 0.0f, 1.0f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0, 1.0);

		//glDisable(GL_CULL_FACE);
		//glDepthMask(GL_FALSE);

		//glUseProgram(skybox->getShader()->getProgram());
		//skybox->Skybox::Render();
		//glUseProgram(0);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glUseProgram(sphere->getShader()->getProgram());

		/// These pass the matricies and the light position to the GPU
		glUniformMatrix4fv(sphere->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
		glUniformMatrix4fv(sphere->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix());
		glUniform3fv(sphere->getShader()->getUniformID("lightPos"), 1, Vec3(-lightSource.x, -lightSource.y, lightSource.z));
		glUniformMatrix4fv(sphere->getShader()->getUniformID("lightSpaceMatrix"), 1, GL_FALSE, lightProj * lightView);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);

		sphere->Render();

		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);

		glUseProgram(botWall->getShader()->getProgram());

		glUniformMatrix4fv(botWall->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
		glUniformMatrix4fv(botWall->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix());
		glUniform3fv(botWall->getShader()->getUniformID("lightPos"), 1, Vec3(-lightSource.x, -lightSource.y, lightSource.z));
		glUniformMatrix4fv(botWall->getShader()->getUniformID("lightSpaceMatrix"), 1, GL_FALSE, lightProj * lightView);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);

		botWall->Render();

		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);

		glUseProgram(backWall->getShader()->getProgram());

		glUniformMatrix4fv(backWall->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
		glUniformMatrix4fv(backWall->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix());
		glUniform3fv(backWall->getShader()->getUniformID("lightPos"), 1, Vec3(-lightSource.x, -lightSource.y, lightSource.z));
		glUniformMatrix4fv(backWall->getShader()->getUniformID("lightSpaceMatrix"), 1, GL_FALSE, lightProj * lightView);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);

		backWall->Render();

		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);

		glUseProgram(leftWall->getShader()->getProgram());

		glUniformMatrix4fv(leftWall->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
		glUniformMatrix4fv(leftWall->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix());
		glUniform3fv(leftWall->getShader()->getUniformID("lightPos"), 1, Vec3(-lightSource.x, -lightSource.y, lightSource.z));
		glUniformMatrix4fv(leftWall->getShader()->getUniformID("lightSpaceMatrix"), 1, GL_FALSE, lightProj * lightView);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);

		leftWall->Render();

		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);

		glUseProgram(debugdepthShader->getProgram());
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		renderQuad();
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
	}
}

void Scene5::OnDestroy() {
	if (camera) delete camera, camera = nullptr;
	if (meshPtr) delete meshPtr, meshPtr = nullptr;
	if (sphere) delete sphere, sphere = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (skybox) delete skybox, skybox = nullptr;
	if (botWall) delete botWall, botWall = nullptr;
	if (backWall) delete backWall, backWall = nullptr;
	if (leftWall) delete leftWall, leftWall = nullptr;
	if (cubeMesh) delete cubeMesh, cubeMesh = nullptr;
	if (depthShader) delete depthShader, depthShader = nullptr;
	if (debugdepthShader) delete debugdepthShader, debugdepthShader = nullptr;
	glDeleteTextures(1, &depthMapTexture);
	glDeleteFramebuffers(1, &depthMapBuffer);
}
