#include "Camera.h"

#include <math.h>
#include <iostream>

namespace tk
{
Camera::Camera(glm::dvec3 initialPos, glm::dvec3 initialDir)
	: m_position(initialPos), m_direction(glm::normalize(initialDir)), m_yaw(0), m_pitch(0)
{
	// Gets the local right vector by using the direction vector and up vector,
	// and getting the "normal" vector of the plane created by these vectors (done with cross product)
	m_left = glm::normalize(glm::cross(glm::dvec3(0.0, 1.0, 0.0), m_direction));
	m_up = glm::normalize(glm::cross(m_direction, m_left));
}

Camera::~Camera()
{
}

void Camera::updateTransform()
{
}

void Camera::rotate(double yaw, double pitch)
{
	m_yaw += yaw;
	m_pitch += pitch;

	//std::cout << m_pitch << ", " << m_yaw << std::endl;

	if (m_pitch < -90.0)
		m_pitch = -90.0;

	if (m_pitch > 90.0)
		m_pitch = 90.0;

	if (m_yaw > 180.0)
		m_yaw -= 360.0;

	if (m_yaw < -180.0)
		m_yaw += 360.0;

	glm::dvec3 newRot = glm::dvec3(0.0);

	newRot.x = cos(glm::radians(-m_pitch)) * cos(glm::radians(m_yaw));
	newRot.y = sin(glm::radians(-m_pitch));
	newRot.z = cos(glm::radians(-m_pitch)) * sin(glm::radians(m_yaw));

	m_direction = glm::normalize(newRot);
	m_left = glm::normalize(glm::cross(glm::dvec3(0.0, 1.0, 0.0), m_direction));
	m_forward = glm::normalize(glm::cross(m_left, glm::dvec3(0.0, 1.0, 0.0)));
	m_up = glm::normalize(glm::cross(m_direction, m_left));
	//std::cout << m_up.x << ", " << m_up.y << ", " << m_up.z << std::endl;
}

glm::mat4 Camera::getView() const
{
	return glm::lookAt(m_position, m_position + m_direction, m_up);
}

void Camera::move(glm::dvec3 direction)
{
	m_position += direction;
}
} // namespace tk