#pragma once
#include <include/glm.h>
#include <include/math.h>

namespace Laborator
{
	class Camera
	{
	public:
		Camera()
		{

			position = glm::vec3(cameraX, cameraY, cameraZ);
			forward = glm::vec3(0, 0, -1);
			up = glm::vec3(0, 1, 0);
			right = glm::vec3(1, 0, 0);
			distanceToTarget = 3.8f;
		}

		Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
		{
			Set(position, center, up);
		}

		~Camera()
		{ }

		void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
		{
			this->position = position;
			forward = glm::normalize(center - position);
			right = glm::cross(forward, up);
			this->up = glm::cross(right, forward);
		}

		void MoveForward(float distance)
		{
			glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
			this->position = this->position + dir*distance;
		}

		void TranslateForward(float distance)
		{
			glm::vec3 dir = glm::normalize(glm::vec3(forward.x, forward.y, forward.z));
			this->position = this->position + dir*distance;
		}

		void TranslateUpword(float distance)
		{
			this->position = this->position + glm::normalize(this->up)*distance;
		}

		void TranslateRight(float distance)
		{
			glm::vec3 dir = glm::normalize(glm::vec3(right.x, 0, right.z));
			this->position = this->position + dir * distance;
		}

		void RotateFirstPerson_OX(float angle)
		{
			forward = glm::normalize(glm::vec3((glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(forward, 1))));
			up = glm::normalize(glm::cross(right, forward));

		}

		void RotateFirstPerson_OY(float angle)
		{
			forward = glm::normalize(glm::vec3((glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1))));
			right = glm::normalize(glm::vec3((glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1))));
		}

		void RotateFirstPerson_OZ(float angle)
		{
			right = glm::normalize(glm::vec3((glm::rotate(glm::mat4(1.0f), angle, forward) * glm::vec4(right, 1))));
			up = glm::normalize(glm::cross(right, forward));

		}

		void RotateThirdPerson_OX(float angle)
		{
			this->TranslateForward(distanceToTarget);
			this->RotateFirstPerson_OX(angle);
			this->TranslateForward(-distanceToTarget);
		}

		void RotateThirdPerson_OY(float angle)
		{
			this->TranslateForward(distanceToTarget);
			this->RotateFirstPerson_OY(angle);
			this->TranslateForward(-distanceToTarget);
		}

		void RotateThirdPerson_OZ(float angle)
		{
			this->TranslateForward(distanceToTarget);
			this->RotateFirstPerson_OZ(angle);
			this->TranslateForward(-distanceToTarget);
		}

		glm::mat4 GetViewMatrix()
		{
			return glm::lookAt(position, position + forward, up);
		}

		glm::vec3 GetTargetPosition()
		{
			return position + forward * distanceToTarget;
		}

	public:
		float distanceToTarget;
		glm::vec3 position;
		glm::vec3 forward;
		glm::vec3 right;
		glm::vec3 up;
		float cameraX = 0, cameraY = 0, cameraZ = 0;
	};
}