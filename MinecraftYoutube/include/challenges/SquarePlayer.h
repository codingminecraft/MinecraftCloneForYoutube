#ifndef SQUARE_PLAYER_H
#define SQUARE_PLAYER_H

namespace MinecraftClone
{
	struct Window;

	namespace SquarePlayer
	{
		void init(const Window& window);
		void destroy();

		void update(float dt);
	}
}

#endif 