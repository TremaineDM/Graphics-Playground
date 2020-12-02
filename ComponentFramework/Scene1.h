#ifndef SCENE1_H
#define SCENE1_H
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

class Scene1 : public Scene {
private:
	Camera* camera;
	DemoObject* demoObject;
	Vec3 lightSource[3];
	Mesh* meshPtr;
	Shader* shaderPtr;
	Texture* texturePtr;
	float GlobalTime;
public:
	explicit Scene1();
	virtual ~Scene1();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};

#endif // SCENE1_H