#include <iostream>
#include <glew.h>
#include "Scene6.h"
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "DemoObject.h"
#include "ObjLoader.h"
#include "Camera.h"
#include "Noise.h"
#include "MMath.h"
#include <algorithm>


Scene6::Scene6()
{
	Debug::Info("Created Scene6: ", __FILE__, __LINE__);
}


Scene6::~Scene6() 
{
	
}

bool Scene6::OnCreate() 
{
	globalTime = 0.0f;
	camera = new Camera();

	ObjLoader::loadOBJ("meshes/Cube.obj");

	gridMesh = new Mesh(GL_PATCHES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	mesh = new Mesh(GL_LINES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);

	matrixShader = new Shader("matrixVert.glsl", "matrixFrag.glsl", "matrixTessC.glsl", "matrixTessE.glsl", "matrixGeo.glsl");
	noiseShader = new Shader("noiseVert.glsl", "noiseFrag.glsl");

	gridCube = new DemoObject(gridMesh, matrixShader, nullptr);
	normalCube = new DemoObject(mesh, noiseShader, nullptr);

	gridCube->setModelMatrix(MMath::translate(Vec3(2, 0, 0)));
	normalCube->setModelMatrix(MMath::translate(Vec3(-2, 0, 0)));

	GLint MaxPatchVertices = 0;
	glGetIntegerv(GL_MAX_PATCH_VERTICES, &MaxPatchVertices);
	printf("Max supported patch vertices %d\n", MaxPatchVertices);
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glEnable(GL_PROGRAM_POINT_SIZE);
	
	noiseTexID = CreateNoise3D();

	return true;
}

void Scene6::OnDestroy() 
{
	if (gridCube) delete gridCube, gridCube = nullptr;
	if (normalCube) delete normalCube, normalCube = nullptr;
	if (gridMesh) delete gridMesh, gridMesh = nullptr;
	if (mesh) delete mesh, mesh = nullptr;
	if (matrixShader) delete matrixShader, matrixShader = nullptr;
	if (noiseShader) delete noiseShader, noiseShader = nullptr;
	if (camera) delete camera, camera = nullptr;
	DeleteNoise3D();
}

void Scene6::HandleEvents(const SDL_Event &sdlEvent) 
{
	camera->HandleEvents(sdlEvent);
}

void Scene6::Update(const float deltaTime) 
{
	globalTime += deltaTime;
}

void Scene6::Render() const {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	/// Clear the screen
	glClearColor(0.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(gridCube->getShader()->getProgram());
	
	glBindTexture(GL_TEXTURE_3D, noiseTexID);
	glUniformMatrix4fv(gridCube->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(gridCube->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix());
	glUniform1f(gridCube->getShader()->getUniformID("time"), globalTime);
	glUniform1f(gridCube->getShader()->getUniformID("fov"), camera->getFOV());
	
	gridCube->Render();

	glUseProgram(normalCube->getShader()->getProgram());

	glUniformMatrix4fv(normalCube->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(normalCube->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix());
	glUniform1f(normalCube->getShader()->getUniformID("time"), globalTime);
	normalCube->Render();


}
