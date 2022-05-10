#include "gameplay/CharacterSystem.h"
#include "gameplay/CharacterController.h"
//#include "gameplay/PlayerController.h"
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

			rb.acceleration.x = camera.forward.x * controller.movementAxis.x;
			rb.acceleration.y = camera.forward.y * controller.movementAxis.x;
			rb.acceleration.z = camera.forward.z * controller.movementAxis.x;
			rb.acceleration.x += camera.right.x * controller.movementAxis.z;
			rb.acceleration.y += camera.right.y * controller.movementAxis.z;
			rb.acceleration.z += camera.right.z * controller.movementAxis.z;
			// The y-axis just changes the player's global y position
			// instead of using their local up axis
			rb.acceleration.y += controller.movementAxis.y;

			if (glm::abs(rb.acceleration.x) > 0 || glm::abs(rb.acceleration.z) > 0 || (glm::abs(rb.acceleration.y) > 0))
			{
				float denominator = glm::inversesqrt(rb.acceleration.x * rb.acceleration.x + rb.acceleration.z * rb.acceleration.z);
				if (glm::abs(rb.acceleration.y) > 0)
				{
					denominator = glm::inversesqrt(rb.acceleration.x * rb.acceleration.x + rb.acceleration.z * rb.acceleration.z + rb.acceleration.y * rb.acceleration.y);
					rb.acceleration.y *= denominator * speed;
				}
				rb.acceleration.x *= denominator * speed;
				rb.acceleration.z *= denominator * speed;
			}

			rb.velocity.x += rb.acceleration.x * Application::deltaTime;
			rb.velocity.y += rb.acceleration.y * Application::deltaTime;
			rb.velocity.z += rb.acceleration.z * Application::deltaTime;

			camera.position.x += rb.velocity.x * Application::deltaTime;
			camera.position.y += rb.velocity.y * Application::deltaTime;
			camera.position.z += rb.velocity.z * Application::deltaTime;

			// If acceleration is 0, apply friction to decelerate the object
			if (glm::all(glm::epsilonEqual(rb.acceleration, glm::vec3(0.0f), 0.001f)))
			{
				rb.velocity.x *= rb.friction * Application::deltaTime;
				rb.velocity.z *= rb.friction * Application::deltaTime;
				rb.velocity.y *= rb.friction * Application::deltaTime;
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

			// Smooth the mouse movements over time
			// Exponential decay function (https://en.wikipedia.org/wiki/Exponential_decay)
			//constexpr float springiness = 50.0f;
			//float delta = 1.0f - glm::exp(glm::log(0.5f) * Application::deltaTime * springiness);
			//float newX = (controller.viewAxis.x - controller._targetViewAxis.x) * delta * Application::deltaTime * controller.movementSensitivity;
			//float newY = (controller.viewAxis.y - controller._targetViewAxis.y) * delta * Application::deltaTime * controller.movementSensitivity;
			//controller.viewAxis.x = newX;
			//controller.viewAxis.y = newY;
			//controller._targetViewAxis = controller.viewAxis;
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