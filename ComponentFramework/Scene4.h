#ifndef SCENE4_H
#define SCENE4_H

#include <vector>
#include "Scene.h"
#include "Vector.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Camera;
class DemoObject;
class Mesh;
class Shader;
class Texture;
class Skybox;

class Scene4 : public Scene {
private:
	Camera* camera;
	Skybox* skybox;
	DemoObject* sphere;
	Vec3 lightSource;
	Mesh* meshPtr;
	Shader* shaderPtr;
	GLuint noiseTexID;

	float GlobalTime;
public:
	explicit Scene4();
	virtual ~Scene4();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};

#endif // SCENE4_H