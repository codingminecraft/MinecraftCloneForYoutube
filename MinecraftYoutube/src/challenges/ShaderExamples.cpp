#include "challenges/ShaderExamples.h"
#include "core.h"
#include "core/Input.h"
#include "core/Window.h"
#include "renderer/ShaderProgram.h"

namespace MinecraftClone
{
	namespace ShaderExamples
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

		static ShaderProgram shaders[4];
		static int currentShaderIndex;

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

		void drawQuad(const ShaderProgram& shader, const glm::vec3& position, const glm::vec3& color, float rotation = 0.0f, const glm::vec3& scale = glm::vec3(1.0f))
		{
			glm::mat4 transform = glm::scale(scale);
			transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
			transform = glm::translate(transform, position);
			shader.uploadMat4("uTransform", transform);
			shader.uploadVec2("u_resolution", glm::vec2(1.0f));
			glm::vec2 localMouse = glm::vec2(Input::mouseX, Input::mouseY);
			shader.uploadVec2("u_mouse", localMouse);
			
			glBindVertexArray(quadVao);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		void destroyQuad()
		{
			glDeleteBuffers(1, &quadVbo);
			glDeleteBuffers(1, &quadEbo);
			glDeleteVertexArrays(1, &quadVao);
		}

		glm::mat4 projection;

		void init(const Window& window)
		{
			shaders[0].compileAndLink("assets/shaders/vertex/basic.glsl", "assets/shaders/fragment/pulsingCircles.glsl");
			shaders[1].compileAndLink("assets/shaders/vertex/basic.glsl", "assets/shaders/fragment/mondrianPaintingMimic.glsl");
			shaders[2].compileAndLink("assets/shaders/vertex/basic.glsl", "assets/shaders/fragment/coolCircleShader.glsl");
			shaders[3].compileAndLink("assets/shaders/vertex/basic.glsl", "assets/shaders/fragment/spinningColorWheel.glsl");

			initQuad();

			float projectionWidth = 6.0f;
			float windowAspect = ((float)window.windowWidth / (float)window.windowHeight);
			float projectionHeight = projectionWidth / windowAspect;
			projection = glm::ortho(-projectionWidth/2.0f, projectionWidth/2.0f, -projectionHeight/2.0f, projectionHeight/2.0f, 0.001f, 100000.0f);
		}

		void destroy()
		{
			destroyQuad();

			for (int i = 0; i < 4; i++)
			{
				shaders[i].destroy();
			}
		}

		void update(float dt)
		{
			static float keyDebounce = 0.0f;
			keyDebounce -= dt;
			shaders[currentShaderIndex].bind();

			static float rotation = 0.0f;
			static int lastRotation = -1;
			rotation += dt * 50.0f;
			float rotationRadians = glm::radians(rotation);
			if ((int)(rotation / 90) > lastRotation + 1)
			{
				lastRotation = (int)(rotation / 90);
				currentShaderIndex = (currentShaderIndex + 1) % 4;
			}
			glm::vec3 eye = glm::vec3(20.0f * glm::sin(rotationRadians), 0.0f, 20.0f * glm::cos(rotationRadians));
			glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
			glm::mat4 view = glm::lookAt(eye, center, up);
			shaders[currentShaderIndex].uploadMat4("uView", view);
			shaders[currentShaderIndex].uploadMat4("uProjection", projection);
			shaders[currentShaderIndex].uploadFloat("u_time", (float)glfwGetTime());

			drawQuad(shaders[currentShaderIndex], glm::vec3(0.0f), glm::vec3(1.0f), 0.0f, glm::vec3(3.0f));

			if (Input::isKeyDown(GLFW_KEY_RIGHT) && keyDebounce < 0.0f)
			{
				currentShaderIndex = (currentShaderIndex + 1) % 4;
				keyDebounce = 0.2f;
			}
		}
	}
}