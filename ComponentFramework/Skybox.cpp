#include "Skybox.h"

Skybox::Skybox(Camera* c, const char* top, const char* bot, const char* left, const char* right, const char* front, const char* back)
{
	camera = c;

	texTop = IMG_Load(top);
	texBot = IMG_Load(bot);
	texLeft = IMG_Load(left);
	texRight = IMG_Load(right);
	texFront = IMG_Load(front);
	texBack = IMG_Load(back);

	LoadSkybox();
	view.loadIdentity();
	ObjLoader::loadOBJ("meshes/Cube.obj");

	mesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shader = new Shader("SkyboxVert.glsl", "SkyboxFrag.glsl");

}

Skybox::~Skybox()
{
	if (mesh) delete mesh, mesh = nullptr;
	if (shader) delete shader, shader = nullptr;
	texTop = nullptr;
	texBot = nullptr;
	texLeft = nullptr;
	texRight = nullptr;
	texFront = nullptr;
	texBack = nullptr;
	//camera = nullptr;
	glDeleteTextures(1, &texID);
}

GLuint Skybox::LoadSkybox()
{
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, texRight->w, texRight->h, 0, GL_RGB, GL_UNSIGNED_BYTE, texRight->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, texLeft->w, texLeft->h, 0, GL_RGB, GL_UNSIGNED_BYTE, texLeft->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, texTop->w, texTop->h, 0, GL_RGB, GL_UNSIGNED_BYTE, texTop->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, texBot->w, texBot->h, 0, GL_RGB, GL_UNSIGNED_BYTE, texBot->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, texBack->w, texBack->h, 0, GL_RGB, GL_UNSIGNED_BYTE, texBack->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, texFront->w, texFront->h, 0, GL_RGB, GL_UNSIGNED_BYTE, texFront->pixels);

	SDL_FreeSurface(texRight);
	SDL_FreeSurface(texLeft);
	SDL_FreeSurface(texTop);
	SDL_FreeSurface(texBot);
	SDL_FreeSurface(texBack);
	SDL_FreeSurface(texFront);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return texID;
}

void Skybox::Render() const
{

	glUniformMatrix4fv(shader->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(shader->getUniformID("viewMatrix"), 1, GL_FALSE, camera->mouseRotationMatrix);

	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

	mesh->Render();

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}

GLuint Skybox::getSkyboxTexture()
{
	return texID;
}
