#include "challenges/TerrainShader.h"
#include "core.h"
#include "core/Input.h"
#include "core/Window.h"
#include "renderer/ShaderProgram.h"

namespace MinecraftClone
{
	namespace TerrainShader
	{
		struct Vertex
		{
			glm::vec3 position;
		};

		static Vertex* terrainVerts = nullptr;
		static uint32* terrainElements = nullptr;
		static int numElements = 0;
		static int numVertices = 0;

		static uint32 terrainVao;
		static uint32 terrainVbo;
		static uint32 terrainEbo;

		static ShaderProgram terrainShader;

		static int maxSubdivision = 20;
		constexpr float terrainSize = 10.0f;

		// Internal functions
		static void calculateVerts();

		void initTerrain()
		{
			numVertices = 0;
			numElements = 0;

			calculateVerts();
			glCreateVertexArrays(1, &terrainVao);
			glBindVertexArray(terrainVao);

			// Set up and buffer element buffer
			glCreateBuffers(1, &terrainEbo);
			glNamedBufferData(terrainEbo, sizeof(uint32) * numElements, terrainElements, GL_STATIC_DRAW);
			glVertexArrayElementBuffer(terrainVao, terrainEbo);

			// Set up and buffer the vertex buffer
			glCreateBuffers(1, &terrainVbo);
			glNamedBufferData(terrainVbo, sizeof(Vertex) * numVertices, terrainVerts, GL_STATIC_DRAW);
			glVertexArrayVertexBuffer(terrainVao, 0, terrainVbo, 0, sizeof(Vertex));

			// Set up and enable attribute 1
			glVertexArrayAttribFormat(terrainVao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
			glVertexArrayAttribBinding(terrainVao, 0, 0);
			glEnableVertexArrayAttrib(terrainVao, 0);
		}

		void drawTerrain(const ShaderProgram& shader)
		{
			shader.uploadMat4("uTransform", glm::identity<glm::mat4>());
			shader.uploadVec2("u_resolution", glm::vec2(5.0f));

			glBindVertexArray(terrainVao);
			glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
		}

		void destroyTerrain()
		{
			glDeleteBuffers(1, &terrainVbo);
			glDeleteBuffers(1, &terrainEbo);
			glDeleteVertexArrays(1, &terrainVao);
			g_memory_free(terrainVerts);
			g_memory_free(terrainElements);
		}

		glm::mat4 projection;

		void init(const Window& window)
		{
			terrainShader.compileAndLink("assets/shaders/vertex/perlinTerrain.glsl", "assets/shaders/fragment/perlinTerrain.glsl");

			initTerrain();

			float windowAspect = ((float)window.windowWidth / (float)window.windowHeight);
			projection = glm::perspective(45.0f, windowAspect, 0.01f, 10000.0f);
		}

		void destroy()
		{
			destroyTerrain();
			terrainShader.destroy();
		}

		void update(float dt)
		{
			static float keyDebounce = 0.0f;
			keyDebounce -= dt;
			terrainShader.bind();

			static float rotation = 0.0f;
			static int lastRotation = -1;
			rotation += dt * 30.0f;
			float rotationRadians = glm::radians(rotation);
			constexpr float maxRadius = (terrainSize / 2.0f)*1.8f;
			glm::vec3 eye = glm::vec3(maxRadius * glm::sin(rotationRadians), 6.0f, maxRadius * glm::cos(rotationRadians));
			glm::vec3 center = glm::vec3(0.0f, -2.0f, 0.0f);
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
			glm::mat4 view = glm::lookAt(eye, center, up);
			terrainShader.uploadMat4("uView", view);
			terrainShader.uploadMat4("uProjection", projection);
			terrainShader.uploadFloat("uGridSize", terrainSize / (float)maxSubdivision);
			terrainShader.uploadFloat("u_time", (float)glfwGetTime());

			drawTerrain(terrainShader);

			if (Input::isKeyDown(GLFW_KEY_UP) && keyDebounce < 0.0f)
			{
				maxSubdivision++;
				destroyTerrain();
				initTerrain();
				keyDebounce = 0.2f;
			}
			else if (Input::isKeyDown(GLFW_KEY_DOWN) && keyDebounce < 0.0f)
			{
				maxSubdivision--;
				maxSubdivision = glm::max(1, maxSubdivision);
				destroyTerrain();
				initTerrain();
				keyDebounce = 0.2f;
			}
		}

		static void calculateVerts()
		{
			constexpr float leftX = -(terrainSize / 2.0f);
			constexpr float bottomY = -(terrainSize / 2.0f);
			float gridSize = terrainSize / (float)maxSubdivision;

			numVertices = maxSubdivision * maxSubdivision;
			terrainVerts = (Vertex*)g_memory_allocate(sizeof(Vertex) * numVertices);
			for (int y = 0; y < maxSubdivision; y++)
			{
				for (int x = 0; x < maxSubdivision; x++)
				{
					terrainVerts[x + (y * maxSubdivision)].position.x = (leftX + 0.5f) + (gridSize * x);
					terrainVerts[x + (y * maxSubdivision)].position.y = -2.0f;
					terrainVerts[x + (y * maxSubdivision)].position.z = (bottomY + 0.5f) + (gridSize * y);
				}
			}

			int numQuads = (maxSubdivision - 1) * (maxSubdivision - 1);
			numElements = numQuads * 6;
			terrainElements = (uint32*)g_memory_allocate(sizeof(uint32) * numElements);
			for (int i = 0; i < numQuads; i++)
			{
				int quadIndex = i + (i / (maxSubdivision - 1));
				// First triangle
				terrainElements[(i * 6) + 0] = quadIndex;
				terrainElements[(i * 6) + 1] = quadIndex + 1;
				terrainElements[(i * 6) + 2] = quadIndex + maxSubdivision;

				// Second triangle
				terrainElements[(i * 6) + 3] = quadIndex + maxSubdivision;
				terrainElements[(i * 6) + 4] = quadIndex + 1;
				terrainElements[(i * 6) + 5] = quadIndex + maxSubdivision + 1;
			}

		}
	}
}