#include "core/Input.h"

namespace MinecraftClone
{
	namespace Input
	{
		// Global Variables
		bool keyPressedData[GLFW_KEY_LAST] = {};
		bool mouseButtonPressedData[GLFW_MOUSE_BUTTON_LAST] = {};
		float mouseX = 0.0f;
		float mouseY = 0.0f;
		float mouseScrollX = 0.0f;
		float mouseScrollY = 0.0f;
		float deltaMouseX = 0.0f;
		float deltaMouseY = 0.0f;

		// Internal Variables
		static float lastMouseX = 0.0f;
		static float lastMouseY = 0.0f;
		static bool firstFrame = true;

		void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key >= 0 && key < GLFW_KEY_LAST)
			{
				keyPressedData[key] = (action == GLFW_PRESS) || (action == GLFW_REPEAT);
			}
		}

		void mouseCallback(GLFWwindow* window, double xpos, double ypos)
		{
			mouseX = (float)xpos;
			mouseY = (float)ypos;
			if (firstFrame)
			{
				// Set these to the mouse position the first frame so that
				// we don't get large erratic values the first frame
				lastMouseX = (float)xpos;
				lastMouseY = (float)ypos;
				firstFrame = false;
			}

			// NOTE: This stuff is just to prevent the camera from jumping if the mouse cursor
			// moves off the window and then back onto the window at a different location
			int windowWidth;
			int windowHeight;
			glfwGetWindowSize(window, &windowWidth, &windowHeight);
			bool cursorLocked = glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
			if (!cursorLocked && 
				(mouseX >= (float)windowWidth || mouseX <= 0.0f || mouseY >= (float)windowHeight || mouseY <= 0.0f))
			{
				// NOTE: If the mouse just came from off the screen and the cursor is not locked
				// then don't count that as a real delta movement
				deltaMouseX = 0.0f;
				deltaMouseY = 0.0f;
			}
			else
			{
				deltaMouseX = ((float)xpos - lastMouseX);
				deltaMouseY = (lastMouseY - (float)ypos);
			}

			lastMouseX = (float)xpos;
			lastMouseY = (float)ypos;
		}

		void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
		{
			if (button >= 0 && button < GLFW_MOUSE_BUTTON_LAST)
			{
				mouseButtonPressedData[button] = action == GLFW_PRESS;
			}
		}

		void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
		{
			mouseScrollX = (float)xoffset;
			mouseScrollY = (float)yoffset;
		}

		void endFrame()
		{
			deltaMouseX = 0.0f;
			deltaMouseY = 0.0f;
		}

		bool isKeyDown(int key)
		{
			if (key >= 0 && key < GLFW_KEY_LAST)
			{
				return keyPressedData[key];
			}

			return false;
		}

		bool isMouseButtonDown(int mouseButton)
		{
			if (mouseButton >= 0 && mouseButton < GLFW_MOUSE_BUTTON_LAST)
			{
				return mouseButtonPressedData[mouseButton];
			}

			return false;
		}
	}
}