#ifndef SHADER_EXAMPLES_H
#define SHADER_EXAMPLES_H

namespace MinecraftClone
{
	struct Window;

	namespace ShaderExamples
	{
		void init(const Window& window);
		void destroy();

		void update(float dt);
	}
}

#endif 