#ifndef TERRAIN_SHADER_H
#define TERRAIN_SHADER_H

namespace MinecraftClone
{
	struct Window;

	namespace TerrainShader
	{
		void init(const Window& window);
		void destroy();

		void update(float dt);
	}
}

#endif 