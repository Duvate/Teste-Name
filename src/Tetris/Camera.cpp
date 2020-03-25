#include "Pch.hpp"
#include "Camera.hpp"

#include "Window.hpp"
#include "GlobalVariables.hpp"
#include "GameState.hpp"

std::unique_ptr<Camera> camera = std::make_unique<Camera>();

Camera::Camera(const vec3 & pos, float pitch, float yaw, float sensitivity, float zoom)
{
	this->pos = pos;
	this->pitch = pitch;
	this->yaw = yaw;
	this->sensitivity = sensitivity;
	this->zoom = zoom;

	front = vec3(0.0f, 0.0f, -1.0f);
	right = vec3(1.0f, 0.0f, 0.0f);
	up = vec3(0.0f, 1.0f, 0.0f);
	worldUp = vec3(0.0f, 1.0f, 0.0f);
}

void Camera::update(bool constrainPitch)
{
	if (gameState == GameState::RUNNING || gameState == GameState::GAME_OVER) // move camera around table
	{
		if (firstMouse)
		{
			firstMouse = false;
			sf::Mouse::setPosition(sf::Vector2i(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), *window);
		}
		sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
		float xOffset = static_cast<float>(mousePos.x - static_cast<int>(WINDOW_WIDTH) / 2), 
			yOffset = static_cast<float>(-mousePos.y + static_cast<int>(WINDOW_HEIGHT) / 2);
		sf::Mouse::setPosition(sf::Vector2i(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), *window);

		xOffset *= sensitivity;
		yOffset *= sensitivity;
		yaw -= xOffset;
		if (yaw > 360.0f) yaw -= 360.0f;
		else if (yaw < 0.0f) yaw += 360.0f;
		pitch += yOffset;

		if (constrainPitch)
		{
			if (pitch > 89.0f) pitch = 89.0f;
			else if (pitch < -89.0f) pitch = -89.0f;
		}

		front.x = cos(radians(yaw)) * cos(radians(pitch));
		front.y = sin(radians(pitch));
		front.z = sin(radians(yaw)) * cos(radians(pitch));
		front = normalize(front);
		right = normalize(cross(front, worldUp));
		up = normalize(cross(front, right));
	}
	else firstMouse = true;
}

void Camera::processMouseScroll(float yOffset)
{
	zoom -= yOffset;

	if (zoom <= 1.0f) zoom = 1.0f;
	else if (zoom >= 90.0f) zoom = 90.0f;
}

mat4 Camera::getViewMatrix()
{
	// Camera will move in a circle around the table, and will go up and down with Up and Down keys
	pos.x = width / 2 - distanceToTable * cos(radians(yaw));
	pos.z = depth / 2 - distanceToTable * sin(radians(yaw));
	return lookAt(pos, pos + front, worldUp);
}