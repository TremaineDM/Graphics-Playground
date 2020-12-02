#ifndef SCENE5_H
#define SCENE5_H

#include <vector>
#include "Scene.h"
#include "Vector.h"
#include "PlaneObject.h"
#include "MMath.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Camera;
class DemoObject;
class Mesh;
class Shader;
class Texture;
class Skybox;

class Scene5 : public Scene {
private:
	Camera* camera;
	Skybox* skybox;
	DemoObject* sphere;
	PlaneObject* botWall;
	PlaneObject* backWall;
	PlaneObject* leftWall;
	Mesh* meshPtr;
	Mesh* cubeMesh;
	Shader* shaderPtr;
	Shader* depthShader;
	Shader* debugdepthShader;

	Vec3 lightSource;
	Matrix4 lightProj;
	Matrix4 lightView;
	
	GLuint depthMapBuffer;
	GLuint depthMapTexture;

	float GlobalTime;

	void RenderShadowTexture(DemoObject* obj) const;
public:
	explicit Scene5();
	virtual ~Scene5();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};

#endif // SCENE5_H