#pragma once

#include "TableDimensions.hpp"

class Camera
{
public:
	Camera(const vec3 & pos = vec3(0, 10, 0), float pitch = -40.0f, float yaw = 270.0f, float sensitivity = 0.04f, float zoom = 45.0f);
	vec3 front, right, up, worldUp;

	void update(bool constrainPitch = true);
	void processMouseScroll(float yOffset);

	mat4 getViewMatrix();

	vec3 pos;
	float pitch, yaw, sensitivity, zoom;
	float verticalSpeed = 8.2f;
	float distanceToTable = 0.0f;
	float stepSpeed = 10.0f;
};
extern std::unique_ptr<Camera> camera;
