#include "gameplay/CharacterSystem.h"
#include "gameplay/CharacterController.h"
#include "renderer/Camera.h"
#include "core/Application.h"
#include "core/Input.h"

namespace MinecraftClone
{
	namespace CharacterSystem
	{
		// Internal Variables
		static float terminalVelocity = 8.0f;

		void update(CharacterController& controller, Camera& camera, Rigidbody& rb)
		{
			float speed = controller.controllerBaseSpeed;
			if (controller.isRunning)
			{
				speed = controller.controllerRunSpeed;
			}

			rb.acceleration = camera.forward * controller.movementAxis.x;
			rb.acceleration += camera.right * controller.movementAxis.z;
			// The y-axis just changes the player's global y position
			// instead of using their local up axis
			rb.acceleration.y += controller.movementAxis.y;

			// Only normalize the acceleration if it is greater than 0. This
			// way we avoid division by 0 errors
			float accelerationMagnitudeSquared = glm::length2(rb.acceleration);
			if (accelerationMagnitudeSquared > 0)
			{
				float denominator = glm::inversesqrt(accelerationMagnitudeSquared);
				rb.acceleration *= denominator * speed;
			}

			rb.velocity += rb.acceleration * Application::deltaTime;
			camera.position += rb.velocity * Application::deltaTime;

			// If acceleration is 0, apply friction to decelerate the object
			if (glm::all(glm::epsilonEqual(rb.acceleration, glm::vec3(0.0f), 0.001f)))
			{
				rb.velocity *= rb.friction * Application::deltaTime;
			}

			if (glm::length2(rb.velocity) >= terminalVelocity)
			{
				rb.velocity = glm::normalize(rb.velocity) * terminalVelocity;
			}

			// Snap the velocity to 0 when it gets close enough
			if (glm::all(glm::epsilonEqual(rb.velocity, glm::vec3(0.0f), 0.001f)))
			{
				rb.velocity = glm::vec3(0.0f);
			}

			// Mouse movements don't need to be smoothed over time.
			// They alread represent a change in position
			controller.viewAxis *= controller.movementSensitivity;

			// Check out this stack overflow for a deeper explanation of 
			// why this is necessary
			// https://gamedev.stackexchange.com/questions/136174/im-rotating-an-object-on-two-axes-so-why-does-it-keep-twisting-around-the-thir
			if (controller.viewAxis.x != 0.0f)
			{
				// Yaw globally
				glm::quat yaw = glm::quat(glm::vec3(0.0f, glm::radians(controller.viewAxis.x), 0.0f));
				camera.orientation = camera.orientation * yaw;
			}

			if (controller.viewAxis.y != 0.0f)
			{
				// Pitch locally (this effects order of operation with quaternion multiplication)
				glm::quat pitch = glm::quat(glm::vec3(glm::radians(-controller.viewAxis.y), 0.0f, 0.0f));
				camera.orientation = pitch * camera.orientation;
			}
		}
	}
}