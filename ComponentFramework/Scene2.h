#ifndef SCENE2_H
#define SCENE2_H

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

class Scene2 : public Scene {
private:
	Camera* camera;
	Skybox* skybox;
	DemoObject* earth;
	DemoObject* moon;
	Vec3 lightSource;
	Mesh* meshPtr;
	Shader* shaderPtr;
	Texture* earthTex;
	Texture* moonTex;
	float GlobalTime;
public:
	explicit Scene2();
	virtual ~Scene2();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};

#endif // SCENE2_H