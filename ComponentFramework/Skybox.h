#ifndef SKYBOX_H
#define SKYBOX_H

#include "glew.h"
#include "SDL.h"
#include "SDL_image.h"
#include "DemoObject.h"
#include "ObjLoader.h"
#include "Camera.h"
#include "MMath.h"

class Skybox : public DemoObject
{
	GLuint texID;

	Matrix4 projection;
	Matrix4 view;

	SDL_Surface* texTop;
	SDL_Surface* texBot;
	SDL_Surface* texLeft;
	SDL_Surface* texRight;
	SDL_Surface* texFront;
	SDL_Surface* texBack;

	Camera* camera;
public:


	Skybox(Camera* c, const char* top, const char* bot, const char* left, const char* right, const char* front, const char* back);
	~Skybox();

	GLuint LoadSkybox();
	void Render() const;
	GLuint getSkyboxTexture();

};

#endif // !SKYBOX_H
