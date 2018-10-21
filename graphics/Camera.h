/*
	@TODO This needs to be fixed, or at least be correct..
*/

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace tk
{
class Camera
{
  public:
	Camera(
		glm::dvec3 initialPos = glm::dvec3(0.0, 0.0, 0.0),
		glm::dvec3 initialDir = glm::dvec3(0.0, 0.0, 1.0));
	~Camera();

	void move(glm::dvec3 direction);
	glm::mat4 getView() const;
	void rotate(double pitch, double yaw);

	inline glm::dvec3 getPosition() const { return m_position; }
	inline glm::dvec3 getDirection() const { return m_forward; }
	inline glm::dvec3 getLeft() const { return m_left; }
	inline glm::dvec3 getUp() const { return m_up; }

	//glm::vec3 forward;

  private:
	void updateTransform();

	glm::dvec3 m_position;

	glm::dvec3 m_forward;
	glm::dvec3 m_left;
	glm::dvec3 m_up;

	double m_yaw, m_pitch;
};
} // namespace tk
