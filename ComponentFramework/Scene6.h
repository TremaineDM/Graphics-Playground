#ifndef SCENE6_H
#define SCENE6_H
#include "Scene.h"

union SDL_Event;
class DemoObject;
class Shader;
class Mesh;
class Camera;

class Scene6 : public Scene {
public:

	DemoObject* gridCube;
	DemoObject* normalCube;
	Shader* matrixShader;
	Shader* noiseShader;
	Mesh* gridMesh;
	Mesh* mesh;
	Camera* camera;
	GLuint noiseTexID;
	float globalTime;

	explicit Scene6();
	virtual ~Scene6();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
};


#endif // SCENE6_H