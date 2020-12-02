#include "PlaneObject.h"

PlaneObject::PlaneObject(Mesh* mesh, Shader* shader, Texture* texture) : DemoObject(mesh, shader, texture)
{

}

void PlaneObject::Update(const float deltaTime)
{
	
	if (VMath::mag(VMath::cross(Vec3(0.0f, 1.0f, 0.0), VMath::normalize(plane))) < 0.001f)
	{
		setModelMatrix(MMath::translate(0.0f, -plane.d - 0.5f, 0.0f) * MMath::scale(8.0f, 0.5f, 8.0f));
		return;
	}

	Vec3 rotationAxis = VMath::normalize(VMath::cross(Vec3(0.0f, 1.0f, 0.0f), VMath::normalize(plane)));
	float degrees = asin(VMath::mag(rotationAxis));
	setModelMatrix(MMath::rotate(degrees * (180.0f / 3.1415f) , rotationAxis) * MMath::translate(0.0f, -plane.d - 0.5f, 0.0f) * MMath::scale(8.0f, 0.5f, 8.0f));
}
