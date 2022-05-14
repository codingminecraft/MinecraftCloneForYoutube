#ifndef MINECRAFT_CLONE_APPLICATION_H
#define MINECRAFT_CLONE_APPLICATION_H

namespace MinecraftClone
{
	struct Window;

	namespace Application
	{
		bool init();
		void run();
		void free();

		const Window& getWindow();

		extern float deltaTime;
	}
}

#endif 