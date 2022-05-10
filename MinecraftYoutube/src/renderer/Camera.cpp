#include "renderer/Camera.h"
#include "core/Application.h"
#include "core/Window.h"

namespace MinecraftClone
{
	const glm::mat4& Camera::calculateViewMatrix()
	{
		glm::mat4 rotationMatrix = glm::toMat4(orientation);
		right = glm::row(rotationMatrix, 0);
		up = glm::row(rotationMatrix, 1);
		glm::vec3 oldForward = forward;
		forward = glm::row(rotationMatrix, 2);

		glm::mat4 translationMatrix = glm::transpose(glm::mat4(
			glm::vec4(1.0f, 0.0f, 0.0f, -position.x),
			glm::vec4(0.0f, 1.0f, 0.0f, -position.y),
			glm::vec4(0.0f, 0.0f, 1.0f, -position.z),
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
		));

		view = rotationMatrix * translationMatrix;
		return view;
	}

	const glm::mat4& Camera::calculateProjectionMatrix()
	{
		projection = glm::perspective(fov, Application::getWindow().getAspectRatio(), 0.1f, 100.0f);
		return projection;
	}
}