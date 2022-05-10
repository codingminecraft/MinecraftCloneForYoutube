#ifndef MINECRAFT_CLONE_CHARACTER_SYSTEM_H
#define MINECRAFT_CLONE_CHARACTER_SYSTEM_H

namespace MinecraftClone
{
	struct Camera;
	struct CharacterController;
	struct Rigidbody;

	namespace CharacterSystem
	{
		void update(CharacterController& controller, Camera& camera, Rigidbody& rb);
	}
}

#endif 