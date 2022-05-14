#include "gameplay/PlayerController.h"
#include "gameplay/CharacterController.h"
#include "core.h"
#include "core/Input.h"
#include "core/Application.h"
#include "core/Window.h"

namespace MinecraftClone
{
	namespace PlayerController
	{
		void update(CharacterController& controller)
		{
			controller.movementAxis.x = Input::isKeyDown(GLFW_KEY_W)
				? -1.0f
				: Input::isKeyDown(GLFW_KEY_S)
				? 1.0f
				: 0.0f;
			controller.movementAxis.z = Input::isKeyDown(GLFW_KEY_D)
				? 1.0f
				: Input::isKeyDown(GLFW_KEY_A)
				? -1.0f
				: 0.0f;
			controller.movementAxis.y = Input::isKeyDown(GLFW_KEY_SPACE)
				? 1.0f
				: Input::isKeyDown(GLFW_KEY_LEFT_SHIFT)
				? -1.0f
				: 0.0f;

			controller.viewAxis.x = Input::deltaMouseX;
			controller.viewAxis.y = Input::deltaMouseY;
		}
	}
}