#include "challenges/SquarePlayer.h"
#include "core.h"
#include "core/Input.h"
#include "core/Window.h"
#include "renderer/ShaderProgram.h"

namespace MinecraftClone
{
	namespace SquarePlayer
	{
		struct Vertex
		{
			glm::vec3 position;
		};

		static std::array<Vertex, 4> quadVerts = {
			// Position
			Vertex{glm::vec3(-0.5f, -0.5f, 0.0f)}, // Bottom-left
			Vertex{glm::vec3(-0.5f,  0.5f, 0.0f)}, // Top-left
			Vertex{glm::vec3(0.5f,  0.5f, 0.0f)}, // Top-right
			Vertex{glm::vec3(0.5f, -0.5f, 0.0f)}, // Bottom-right
		};

		static std::array<uint32, 6> quadElements = {
			0, 1, 2,   0, 2, 3,
		};

		static uint32 quadVao;
		static uint32 quadVbo;
		static uint32 quadEbo;

		static ShaderProgram basicShader;

		void initQuad()
		{
			glCreateVertexArrays(1, &quadVao);
			glBindVertexArray(quadVao);

			// Set up and buffer element buffer
			glCreateBuffers(1, &quadEbo);
			glNamedBufferData(quadEbo, sizeof(quadElements), quadElements.data(), GL_STATIC_DRAW);
			glVertexArrayElementBuffer(quadVao, quadEbo);

			// Set up and buffer the vertex buffer
			glCreateBuffers(1, &quadVbo);
			glNamedBufferData(quadVbo, sizeof(quadVerts), quadVerts.data(), GL_STATIC_DRAW);
			glVertexArrayVertexBuffer(quadVao, 0, quadVbo, 0, sizeof(Vertex));

			// Set up and enable attribute 1
			glVertexArrayAttribFormat(quadVao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
			glVertexArrayAttribBinding(quadVao, 0, 0);
			glEnableVertexArrayAttrib(quadVao, 0);
		}

		void drawQuad(const glm::vec3& position, const glm::vec3& color, float rotation = 0.0f, const glm::vec3& scale = glm::vec3(1.0f))
		{
			glm::mat4 transform = glm::scale(scale);
			transform = glm::translate(transform, position);
			transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
			basicShader.uploadVec3("uColor", color);
			basicShader.uploadMat4("uTransform", transform);
			glBindVertexArray(quadVao);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		void destroyQuad()
		{
			glDeleteBuffers(1, &quadVbo);
			glDeleteBuffers(1, &quadEbo);
			glDeleteVertexArrays(1, &quadVao);
		}

		static glm::vec3 playerPos = glm::vec3(3.0f, 1.5f, 0.0f);
		static glm::vec3 playerSpeed = glm::vec3(0.8f, 0.8f, 0.0f);
		static glm::vec3 playerColor = glm::vec3(230.0f/255.0f, 159.0f/255.0f, 102.0f/255.0f);
		static float playerRotation = 0.0f;
		static float keyDebounce = 0.0f;
		glm::mat4 projection;

		void init(const Window& window)
		{
			if (!basicShader.compileAndLink("assets/shaders/vertex/basic.glsl", "assets/shaders/fragment/basic.glsl"))
			{
				basicShader.destroy();
				g_logger_error("Failed to compile the shader program, exiting early.");
			}

			initQuad();

			float projectionWidth = 6.0f;
			float windowAspect = ((float)window.windowWidth / (float)window.windowHeight);
			float projectionHeight = projectionWidth / windowAspect;
			projection = glm::ortho(0.0f, projectionWidth, 0.0f, projectionHeight, 0.0001f, 10000.0f);
		}

		void destroy()
		{
			destroyQuad();
		}

		void update(float dt)
		{
			keyDebounce -= dt;
			basicShader.bind();

			glm::vec3 eye = glm::vec3(0.0f, 0.0f, 20.0f);
			glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
			glm::mat4 view = glm::lookAt(eye, center, up);
			basicShader.uploadMat4("uView", view);
			basicShader.uploadMat4("uProjection", projection);

			// Check if the user pressed any of the keys 1-6
			// If they did delete the challenge we were on and setup up the next challenge
			static bool rotate = false;
			if (rotate)
			{
				playerRotation += dt * 40.0f;
			}
			drawQuad(playerPos, playerColor, playerRotation);

			if (Input::isKeyDown(GLFW_KEY_E) && keyDebounce < 0)
			{
				rotate = !rotate;
				keyDebounce = 0.3f;
			}

			if (Input::isKeyDown(GLFW_KEY_D))
			{
				playerPos.x += dt * playerSpeed.x;
			}
			else if (Input::isKeyDown(GLFW_KEY_A))
			{
				playerPos.x -= dt * playerSpeed.x;
			}

			if (Input::isKeyDown(GLFW_KEY_W))
			{
				playerPos.y += dt * playerSpeed.y;
			}
			else if (Input::isKeyDown(GLFW_KEY_S))
			{
				playerPos.y -= dt * playerSpeed.y;
			}
		}
	}
}