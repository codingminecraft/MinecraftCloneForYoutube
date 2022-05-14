#include "core.h"
#include "core/Application.h"

using namespace MinecraftClone;

int main()
{
	// Try to initialize the app, if it fails exit early
	if (!Application::init())
	{
		g_logger_error("Failed to initialize the game. See the logs above for more details.");
		return -1;
	}

	Application::run();
	Application::free();

	return 0;
}