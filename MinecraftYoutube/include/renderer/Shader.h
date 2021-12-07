#ifndef MINECRAFT_CLONE_SHADER_H
#define MINECRAFT_CLONE_SHADER_H
#include "core.h"

namespace MinecraftClone
{
	struct Shader
	{
		uint32 programId;

		void bind();
		void compile();
	};
}

#endif 