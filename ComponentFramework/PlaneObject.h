#pragma once
#include "DemoObject.h"
#include "Plane.h"
#include "MMath.h"
#include "VMath.h"

class PlaneObject : public DemoObject
{
private:

public:

	Plane plane;

	PlaneObject(Mesh* mesh, Shader* shader, Texture* texture);

	void Update(const float deltaTime) override;


};

