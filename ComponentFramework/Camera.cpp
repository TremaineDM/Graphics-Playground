#include "Camera.h"
#include "MMath.h"

Vec3 Camera::GetMouseVector(int x, int y)
{
	Vec3 mousePosition = Vec3(static_cast<float>(x), static_cast<float>(y), 0.0f);
	Vec3 v = invNDC * mousePosition;
	
	float xSquared = v.x * v.x;
	float ySquared = v.y * v.y;
	if (xSquared + ySquared <= 0.5f) {
		v.z = sqrt(1.0f - (xSquared + ySquared));
	}
	else {
		v.z = 0.5f / sqrt(xSquared + ySquared);
		v = VMath::normalize(v);
	}
	
	return v;
}

void Camera::OnMouseMove(int x, int y)
{
	if (mouseDown == false)
	{
		return;
	}

	mouseEnd = GetMouseVector(x, y);
	float cosAngle = VMath::dot(mouseStart, mouseEnd);
	float angle = acos(cosAngle) * 180.0f / M_PI; /// acos() returns radians must convert to degrees
	Vec3 rotAxis = VMath::cross(mouseStart, mouseEnd);

	mouseRotationMatrix = MMath::rotate(angle, rotAxis) * mouseRotationMatrix;
	setViewMatrix(translation * mouseRotationMatrix);
	/// reset for the next mousemove
	mouseStart = mouseEnd;
}

Camera::Camera() {
	fov = 45.0f;
	setProjectionMatrix(MMath::perspective(fov, (16.0f / 9.0f), 0.01f, 100.0f));
	setViewMatrix(MMath::lookAt(Vec3(0.0, 0.0, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f)));
	translation = MMath::translate(Vec3(0.0f, 0.0f, -10.0f));
	//mouseRotationMatrix.loadIdentity();
	//setViewMatrix(translation * mouseRotationMatrix);

	mouseStart = Vec3(0.0f);
	mouseEnd = Vec3(0.0f);
	mouseDown = false;

	int viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);

	invNDC = MMath::inverse(MMath::viewportNDC(viewport[2], viewport[3]));

}

Camera::~Camera() {}

void Camera::HandleEvents(const SDL_Event& event)
{
	if (event.type == SDL_EventType::SDL_MOUSEBUTTONDOWN)
	{
		mouseDown = true;
		mouseStart = GetMouseVector(event.button.x, event.button.y);
	}
	else if (event.type == SDL_EventType::SDL_MOUSEBUTTONUP)
	{
		mouseDown = false;
	}
	
	if (event.type == SDL_EventType::SDL_MOUSEMOTION &&
		SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		OnMouseMove(event.button.x, event.button.y);
	}

	if (event.type == SDL_MOUSEWHEEL && event.wheel.y > 0)
	{
		fov -= 1.0f;
		setProjectionMatrix(MMath::perspective(fov, (16.0f / 9.0f), 0.01f, 100.0f));

	} 
	else if (event.wheel.y < 0)
	{
		fov += 1.0f;
		setProjectionMatrix(MMath::perspective(fov, (16.0f / 9.0f), 0.01f, 100.0f));
	}

	if (event.key.keysym.scancode == SDL_SCANCODE_P)
	{
		fov = 45.0f;
		setProjectionMatrix(MMath::perspective(45.0f, (16.0f / 9.0f), 0.01f, 100.0f));
		setViewMatrix(MMath::lookAt(Vec3(0.0, 0.0, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f)));
		mouseRotationMatrix.loadIdentity();
		
	}

}

void Camera::Update(const float deltaTime)
{

}
