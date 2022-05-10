#ifndef SQUARE_PLAYER_H
#define SQUARE_PLAYER_H

namespace MinecraftClone
{
	struct Window;
	struct Camera;

	namespace Cubes
	{
		void init();
		void destroy();

		void update(const Camera& camera);
	}
}

#endif 