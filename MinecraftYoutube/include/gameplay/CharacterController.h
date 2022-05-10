#ifndef MINECRAFT_CLONE_CHARACTER_CONTROLLER_H
#define MINECRAFT_CLONE_CHARACTER_CONTROLLER_H
#include "core.h"

namespace MinecraftClone
{
	struct CharacterController
	{
		float controllerBaseSpeed;
		float controllerRunSpeed;
		float movementSensitivity;

		glm::vec3 movementAxis;
		glm::vec3 viewAxis;
		glm::vec3 _targetViewAxis;
		bool isRunning;
		bool lockedToCamera;
	};

	// TODO: Move this into physics library once it gets created in the physics episode
	struct Rigidbody
	{
		glm::vec3 velocity;
		glm::vec3 acceleration;
		float friction;
	};
}

#endif 