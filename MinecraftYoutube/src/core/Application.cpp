#include "core/Application.h"
#include "core.h"
#include "core/Window.h"
#include "core/Input.h"

// TODO: Temporary includes
#include "challenges/Cubes.h"
#include "gameplay/CharacterController.h"
#include "gameplay/CharacterSystem.h"
#include "gameplay/PlayerController.h"
#include "renderer/Camera.h"

namespace MinecraftClone
{
	namespace Application
	{
		// Global variables
		// Initialize this to 60FPS so we don't start the first frame with any
		// division by 0 errors or something
		float deltaTime = 1.0f / 60.0f;

		// Internal variables
		static Window* window = nullptr;
		static constexpr int windowWidth = 1920;
		static constexpr int windowHeight = 1080;
		static const char* windowTitle = "Minecraft Clone";

		// Internal Functions
		void GLAPIENTRY errorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
		void glfwErrorCallback(int error, const char* description);

		bool init()
		{
			glfwSetErrorCallback(glfwErrorCallback);
			if (!glfwInit())
			{
				g_logger_error("GLFW failed to initialize");
				return false;
			}

			window = Window::createWindow(windowWidth, windowHeight, windowTitle);
			if (window == nullptr)
			{
				g_logger_error("Failed to create GLFW window");
				glfwTerminate();
				return false;
			}
			window->installMainCallbacks();

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				g_logger_error("Failed to initialize GLAD.");
				glfwTerminate();
				return false;
			}

			// During init, enable debug output
			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(errorMessageCallback, 0);

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);

			glViewport(0, 0, windowWidth, windowHeight);

			// Set the window cursor to be locked
			glfwSetInputMode((GLFWwindow*)window->nativeWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			// Initialize our game stuff now
			Cubes::init();

			return true;
		}

		void run()
		{
			float frameStart = 0.0f;

			Camera camera = {};
			// We need to initialize the orientation of the camera
			glm::vec3 forward = glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));
			glm::vec3 right = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), forward);
			glm::vec3 up = glm::cross(forward, right);
			glm::mat4 rotMatrix = glm::transpose(glm::mat4(
				glm::vec4(right,            0.0f),
				glm::vec4(up,               0.0f),
				glm::vec4(forward,          0.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
			));
			camera.orientation = glm::toQuat(rotMatrix);
			// The camera projection matrix should only need to be calculated once
			// unless the FOV or aspect ratio changes. I'll ignore those for now.
			camera.fov = 70.0f;
			camera.calculateProjectionMatrix();

			CharacterController controller = {};
			controller.controllerBaseSpeed = 66.0f;
			controller.controllerRunSpeed = controller.controllerBaseSpeed * 2.0f;
			controller.movementSensitivity = 0.1f;

			Rigidbody rb = {};
			rb.friction = 0.1f;

			while (!glfwWindowShouldClose(window->nativeWindow))
			{
				deltaTime = (float)(glfwGetTime() - (double)frameStart);
				frameStart = (float)glfwGetTime();

				glClearColor(39.0f / 255.0f, 40.0f / 255.0f, 34.0f / 255.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				// Update the player first, so that we get the input settled
				PlayerController::update(controller);
				// Then update the player data using the inputs we got
				CharacterSystem::update(controller, camera, rb);
				// Finally calculate our new view matrix and draw our scene
				camera.calculateViewMatrix();
				Cubes::update(camera);

				if (Input::isKeyDown(GLFW_KEY_ESCAPE))
				{
					glfwSetInputMode((GLFWwindow*)window->nativeWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				}
				else if (Input::isKeyDown(GLFW_KEY_ENTER))
				{
					glfwSetInputMode((GLFWwindow*)window->nativeWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				}

				glfwSwapBuffers(window->nativeWindow);

				Input::endFrame();
				glfwPollEvents();
			}
		}

		void free()
		{
			// Try to destroy things in the reverse order of initialization
			// to minimize any weird dependency issues during de-initialization
			Cubes::destroy();

			Window::freeWindow(window);
			window = nullptr;

			glfwTerminate();
		}

		const Window& getWindow()
		{
			return *window;
		}

		// ------------ Internal Functions ------------
		void GLAPIENTRY errorMessageCallback(
			GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
		{
			if (type == GL_DEBUG_TYPE_ERROR)
			{
				g_logger_error("GL CALLBACK: **GL ERROR** type = 0x%x, severity = 0x%x, message = %s\n",
					type, severity, message);
			}
		}

		void glfwErrorCallback(int error, const char* description)
		{
			g_logger_error("GLFW CALLBACK: **GLFW ERROR** Error: %d description: %s\n", error, description);
		}
	}
}
