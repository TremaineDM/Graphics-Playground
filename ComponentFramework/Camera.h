#ifndef CAMERA_H
#define CAMERA_H
#include "Vector.h"
#include "VMath.h"
#include "Matrix.h"
#include "SDL.h"
#include "glew.h"


using namespace MATH;
class Camera {
private:

	Matrix4 projection;
	Matrix4 view;
	Matrix4 invNDC;
	Matrix4 translation;
	Vec3 mouseStart, mouseEnd;
	bool mouseDown;
	float fov;
	Vec3 GetMouseVector(int x, int y);
	void OnMouseMove(int x, int y);

public:
	Matrix4 mouseRotationMatrix;

	inline Matrix4 getProjectionMatrix() const { return projection; }
	inline Matrix4 getViewMatrix() const { return view; }
	inline void setProjectionMatrix(Matrix4 projection_){ projection = projection_; }
	inline void setViewMatrix(Matrix4 view_) { view = view_;  }
	inline float getFOV() { return fov; }
	Camera();
	~Camera();
	void HandleEvents(const SDL_Event& event);
	void Update(const float deltaTime);
};

#endif



