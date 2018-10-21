#include "Camera.h"

#include <math.h>
#include <iostream>

namespace tk
{
Camera::Camera(glm::dvec3 initialPos, glm::dvec3 initialDir)
	: m_position(initialPos), m_forward(glm::normalize(initialDir))
{
	// Gets the local right vector by using the direction vector and up vector,
	// and getting the "normal" vector of the plane created by these vectors (done with cross product)
	m_yaw = glm::asin(initialDir.y);
	m_pitch = glm::pow(glm::atan(initialDir.x, initialDir.z), 2);

	m_left = glm::normalize(glm::cross(glm::dvec3(0.0, 1.0, 0.0), m_forward));
	m_up = glm::normalize(glm::cross(m_left, m_forward));
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

	if (m_pitch < -90.0)
		m_pitch = -90.0;

	if (m_pitch > 90.0)
		m_pitch = 90.0;

	if (m_yaw > 180.0)
		m_yaw -= 360.0;

	if (m_yaw < -180.0)
		m_yaw += 360.0;

	glm::dvec3 newRot = glm::dvec3(0.0);
	// Should not have a minus sign...
	newRot.x = cos(glm::radians(-m_pitch)) * cos(glm::radians(m_yaw));
	newRot.y = sin(glm::radians(-m_pitch));
	newRot.z = cos(glm::radians(-m_pitch)) * sin(glm::radians(m_yaw));

	m_forward = glm::normalize(newRot);
	m_left = glm::normalize(glm::cross(glm::dvec3(0.0, 1.0, 0.0), m_forward));
	m_up = glm::normalize(glm::cross(m_forward, m_left)); // Should be m_left, m_forward
}

glm::mat4 Camera::getView() const
{
	return glm::lookAt(m_position, m_position + m_forward, m_up);
}

void Camera::move(glm::dvec3 direction)
{
	m_position += direction;
}
} // namespace tk