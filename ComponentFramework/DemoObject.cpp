#include "DemoObject.h"
#include "MMath.h"


DemoObject::DemoObject(): mesh(nullptr), shader(nullptr), texture(nullptr)
{
}

DemoObject::DemoObject(Mesh *mesh_, Shader *shader_, Texture *texture_):
	mesh(mesh_), shader(shader_), texture(texture_) {

	modelMatrixID = shader->getUniformID("modelMatrix");
	normalMatrixID = shader->getUniformID("normalMatrix");

}

DemoObject::~DemoObject() 
{
	//if (mesh) delete mesh, mesh = nullptr;
	//if (texture) delete texture, texture = nullptr;
	//if (shader) delete shader, shader = nullptr;
}

bool DemoObject::OnCreate() { return true;  } /// Just a stub
void DemoObject::OnDestroy() {}				  /// Just a stub
void DemoObject::Update(float deltaTime_) {}  /// Just a stub

void DemoObject::Render() const {

	/// This is some fancy code.  Assigning a 4x4 matrix to a 3x3 matrix
	/// just steals the upper 3x3 of the 4x4 and assigns thoses values 
	/// to the 3x3.  
	Matrix3 normalMatrix = MMath::transpose(MMath::inverse(modelMatrix));

	/*std::cout << "model" << std::endl;
	modelMatrix.print();
	std::cout << "inverse" << std::endl;
	MMath::inverse(modelMatrix).print();
	std::cout << "transpose" << std::endl;
	MMath::transpose(MMath::inverse(modelMatrix)).print();
	std::cout << "normal" << std::endl;
	normalMatrix.print();*/

	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, modelMatrix);
	glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, normalMatrix);
	if (texture) {
		glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	}

	mesh->Render();

	
}

void DemoObject::HandleEvents(const SDL_Event &event) {} /// Just a stub