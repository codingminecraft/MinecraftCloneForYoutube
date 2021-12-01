#include "core.h"
#include "core/Window.h"
#include "core/Input.h"

using namespace MinecraftClone;

const int windowWidth = 1920;
const int windowHeight = 1080;
const char* windowTitle = "OpenGL Template";

int exitWithError(const char* msg)
{
	printf(msg);
	glfwTerminate();
	return -1;
}

int main()
{
	glfwInit();

	Window* window = Window::createWindow(windowWidth, windowHeight, windowTitle);
	if (window == nullptr)
	{
		return exitWithError("Failed to create GLFW window\n");
	}
	window->installMainCallbacks();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return exitWithError("Failed to initialize GLAD.\n");
	}

	glViewport(0, 0, windowWidth, windowHeight);
	while (!glfwWindowShouldClose(window->nativeWindow))
	{
		glClearColor(250.0f / 255.0f, 119.0f / 255.0f, 110.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (Input::isKeyDown(GLFW_KEY_E))
		{
			printf("E key is being pressed.\n");
		}

		if (Input::mouseX >= 800.0f)
		{
			printf("Mouse is at the right part of the window.\n");
		}

		if (Input::isKeyDown(GLFW_KEY_ESCAPE))
		{
			window->close();
		}

		glfwSwapBuffers(window->nativeWindow);
		glfwPollEvents();
	}

	Window::freeWindow(window);
	glfwTerminate();
	return 0;
}

