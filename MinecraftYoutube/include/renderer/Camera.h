#ifndef MINECRAFT_CLONE_CAMERA_H
#define MINECRAFT_CLONE_CAMERA_H
#include "core.h"

namespace MinecraftClone
{
	struct Camera
	{
		// TODO: Replace this with an entity representing the
		// camera that holds a Transform containing a position
		glm::vec3 position;
		glm::quat orientation;
		// NOTE: Techinally the forward, right and up vectors
		// can be obtained from orientation. However, we can
		// cache them to avoid recomputing an entire matrix
		// every time we need a directional vector.
		glm::vec3 forward;
		glm::vec3 right;
		glm::vec3 up;
		glm::mat4 projection;
		glm::mat4 view;
		float fov;

		// This just calculates the view and then
		// returns the member variable view
		const glm::mat4& calculateViewMatrix();

		// This just calculates projection and then
		// returns member variable projection
		const glm::mat4& calculateProjectionMatrix();
	};
}

#endif 