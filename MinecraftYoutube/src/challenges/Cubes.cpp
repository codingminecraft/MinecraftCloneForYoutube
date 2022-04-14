#include "challenges/Cubes.h"
#include "core.h"
#include "core/Input.h"
#include "core/Window.h"
#include "renderer/ShaderProgram.h"

namespace MinecraftClone
{
	namespace Cubes
	{
		struct Vertex
		{
			glm::vec3 position;
			glm::vec2 texCoords;
			glm::vec3 normal;
			glm::vec3 tangent;
			glm::vec3 bitangent;
		};

		struct Texture
		{
			std::string filepath;
			uint32 textureId;
			int32 width;
			int32 height;
			int32 numChannels;
		};

		// NOTE: This is an extremely inefficient way to do this
		// but we will cover a much better method in a future episode

		// The 8 vertices will look like this:
		//   v4 ----------- v5
		//   /|            /|      Axis orientation
		//  / |           / |
		// v0 --------- v1  |      y
		// |  |         |   |      |
		// |  v6 -------|-- v7     +--- x
		// | /          |  /      /
		// |/           | /      z
		// v2 --------- v3
		//
		// Where v0, v4, v5, v1 is the top face
		struct Cube
		{
			std::array<Vertex, 36> vertices;
			uint32 vao;
			uint32 vbo;
		};

		// NOTE: We can't use these indices because the texture
		// data changes from vertex to vertex
		// But we can use them to generate the 36 vertices
		static std::array<uint32, 36> cubeElements = {
			// Each set of 6 indices represents one quad
			1, 0, 2,    3, 1, 2,    // Front face
			5, 1, 3,    7, 5, 3,    // Right face
			7, 6, 4,    5, 7, 4,    // Back face
			0, 4, 6,    2, 0, 6,    // Left face
			5, 4, 0,    1, 5, 0,    // Top face
			3, 2, 6,    7, 3, 6     // Bottom face
		};

		// NOTE: Normals, tangents, and bitangents are not
		// part of the challenges
		static std::array<glm::vec3, 6> normals = {
			glm::vec3(0, 0, 1),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, -1),
			glm::vec3(-1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, -1, 0)
		};

		static std::array<glm::vec3, 6> tangents = {
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, 1),
			glm::vec3(-1, 0, 0),
			glm::vec3(0, 0, -1),
			glm::vec3(1, 0, 0),
			glm::vec3(-1, 0, 0)
		};

		static std::array<glm::vec3, 6> biTangents = {
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, -1, 0),
			glm::vec3(0, -1, 0),
			glm::vec3(0, 0, 1),
			glm::vec3(0, 0, -1)
		};

		static std::vector<std::string> textureFilepaths;
		static Cube defaultCube;
		static ShaderProgram texturedCubeShader;
		static std::vector<Texture> textures;
		static Texture normalTexture;
		static std::vector<glm::vec3> cubePositions;
		static std::vector<int> cubeTextures;

		static glm::mat4 projection;

		void createDefaultCube()
		{
			// Define the 8 unique positions for a cube according to the diagram above
			std::array<glm::vec3, 8> cubeVertices;
			cubeVertices[0] = glm::vec3(-0.5f, 0.5f, 0.5f);
			cubeVertices[1] = glm::vec3(0.5f, 0.5f, 0.5f);
			cubeVertices[2] = glm::vec3(-0.5f, -0.5f, 0.5f);
			cubeVertices[3] = glm::vec3(0.5f, -0.5f, 0.5f);

			cubeVertices[4] = glm::vec3(-0.5f, 0.5f, -0.5f);
			cubeVertices[5] = glm::vec3(0.5f, 0.5f, -0.5f);
			cubeVertices[6] = glm::vec3(-0.5f, -0.5f, -0.5f);
			cubeVertices[7] = glm::vec3(0.5f, -0.5f, -0.5f);

			// Tex-coords always loop with the triangle going:
			//		top-right, top-left, bottom-left
			//		top-right, bottom-right, bottom-left
			std::array<glm::vec2, 6> texCoords;
			texCoords[0] = glm::vec2(1.0f, 1.0f);
			texCoords[1] = glm::vec2(0.0f, 1.0f);
			texCoords[2] = glm::vec2(0.0f, 0.0f);

			texCoords[4] = glm::vec2(1.0f, 1.0f);
			texCoords[3] = glm::vec2(1.0f, 0.0f);
			texCoords[5] = glm::vec2(0.0f, 0.0f);

			// Generate the vertices for the default cube
			for (int vertexIndex = 0; vertexIndex < cubeElements.size(); vertexIndex++)
			{
				defaultCube.vertices[vertexIndex].position = cubeVertices[cubeElements[vertexIndex]];
				defaultCube.vertices[vertexIndex].texCoords = texCoords[vertexIndex % 6];

				// NOTE: The normal, tangent, and bitangents are just extra stuff for lighting
				// in the video. You don't have to worry about these for the challenges :)
				defaultCube.vertices[vertexIndex].normal = normals[vertexIndex / 6];
				defaultCube.vertices[vertexIndex].tangent = tangents[vertexIndex / 6];
				defaultCube.vertices[vertexIndex].bitangent = biTangents[vertexIndex / 6];
			}

			glGenVertexArrays(1, &defaultCube.vao);
			glBindVertexArray(defaultCube.vao);

			// Set up and buffer the vertex buffer (OpenGL 3.3 method)
			glGenBuffers(1, &defaultCube.vbo);
			glBindBuffer(GL_ARRAY_BUFFER, defaultCube.vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * defaultCube.vertices.size(), defaultCube.vertices.data(), GL_STATIC_DRAW);

			// Set up and enable attribute 0 (OpenGL 3.3 method)
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
			glEnableVertexAttribArray(0);

			// Set up and enable attribute 1 (OpenGL 3.3 method)
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
			glEnableVertexAttribArray(1);

			// NOTE: These attributes are just extra stuff to enable lighting for a cool demo for the video
			// you don't have to worry about any of these attributes for these challenges :)
			// Set up and enable attribute 2 (OpenGL 3.3 method)
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
			glEnableVertexAttribArray(2);

			// Set up and enable attribute 2 (OpenGL 3.3 method)
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
			glEnableVertexAttribArray(3);

			// Set up and enable attribute 2 (OpenGL 3.3 method)
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
			glEnableVertexAttribArray(4);

		}

		void drawCube(const glm::vec3& position, const Texture& texture)
		{
			// NOTE: We'll cover rotation and scaling of 3D objects another time
			// Upload transform
			glm::mat4 transform = glm::translate(position);
			texturedCubeShader.uploadMat4("uTransform", transform);

			// Upload texture
			// Bind our texture to texture slot 0
			int textureSlot = 0;
			glActiveTexture(GL_TEXTURE0 + textureSlot);
			glBindTexture(GL_TEXTURE_2D, texture.textureId);
			// DSA version of the two lines above (requires OpenGL 4.5+)
			//glBindTextureUnit(textureSlot, texture.textureId);
			// Upload texture slot 0 to our uniform uTexture
			texturedCubeShader.uploadInt("uTexture", textureSlot);

			// Draw our cube
			glBindVertexArray(defaultCube.vao);
			glDrawArrays(GL_TRIANGLES, 0, (GLsizei)defaultCube.vertices.size());
		}

		void destroyDefaultCube()
		{
			glDeleteBuffers(1, &defaultCube.vbo);
			glDeleteVertexArrays(1, &defaultCube.vao);
		}

		Texture createTexture(const std::string& filepath, bool pixelated = false)
		{
			Texture res;
			res.filepath = filepath;
			uint8* pixels = stbi_load(res.filepath.c_str(), &res.width, &res.height, &res.numChannels, 0);

			g_logger_assert((pixels != nullptr), "STB failed to load image: %s\n-> STB Failure Reason: %s", res.filepath.c_str(), stbi_failure_reason());

			int bytesPerPixel = res.numChannels;
			uint32 textureFormat = GL_RGBA;
			uint32 internalFormat = GL_RGBA32F;
			if (bytesPerPixel == 3)
			{
				textureFormat = GL_RGB;
				internalFormat = GL_RGB32F;
			}
			else if (bytesPerPixel != 4)
			{
				g_logger_error("Unknown number of channels '%d' in image '%s'.", res.numChannels, res.filepath.c_str());
				stbi_image_free(pixels);
				return Texture{};
			}

			// Generate and bind the texture object
			glGenTextures(1, &res.textureId);
			glBindTexture(GL_TEXTURE_2D, res.textureId);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, res.width, res.height, 0, textureFormat, GL_UNSIGNED_BYTE, pixels);

			stbi_image_free(pixels);

			return res;
		}

		void destroyTexture(Texture& texture)
		{
			glDeleteTextures(1, &texture.textureId);
		}

		void init(const Window& window)
		{
			if (!texturedCubeShader.compileAndLink("assets/shaders/vertex/cube.glsl", "assets/shaders/fragment/cube.glsl"))
			{
				texturedCubeShader.destroy();
				g_logger_error("Failed to compile the shader program.");
			}

			createDefaultCube();

			float windowAspect = ((float)window.windowWidth / (float)window.windowHeight);
			float fov = 70.0f;
			float zNear = 0.1f;
			float zFar = 10'000.0f;
			projection = glm::perspective(fov, windowAspect, zNear, zFar);

			for (auto& filepath : std::filesystem::directory_iterator("assets/images"))
			{
				if (filepath.path().stem().string() != "normal")
				{
					textures.push_back(createTexture(filepath.path().string(), true));
				}
			}

			// Initialize some cubes
			for (int x = 0; x < 10; x++)
			{
				for (int z = 0; z < 10; z++)
				{
					int tex = rand() % textures.size();
					cubePositions.push_back(glm::vec3(x - 5, 0, z - 5));
					cubeTextures.push_back(tex);

					// Add a few random blocks poking up
					if (rand() % 10 > 8)
					{
						int tex = rand() % textures.size();
						cubePositions.push_back(glm::vec3(x - 5, rand() % 3, z - 5));
						cubeTextures.push_back(tex);
					}
				}
			}

			normalTexture = createTexture("assets/images/normal.jpg");
		}

		void destroy()
		{
			destroyDefaultCube();

			for (auto& texture : textures)
			{
				destroyTexture(texture);
			}
		}

		void update(float dt)
		{
			texturedCubeShader.bind();

			// Rotate the eye a little bit every frame
			static glm::vec3 eye = glm::vec3();
			static float eyeRotation = 45.0f;
			if (!Input::isKeyDown(GLFW_KEY_SPACE)) 
				eyeRotation += 30.0f * dt;
			eye = glm::vec3(glm::sin(glm::radians(eyeRotation)) * 7.0f, 5.0f, glm::cos(glm::radians(eyeRotation)) * 7.0f);

			glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
			glm::mat4 view = glm::lookAt(eye, center, up);
			texturedCubeShader.uploadMat4("uView", view);
			texturedCubeShader.uploadMat4("uProjection", projection);

			// NOTE: This texture is not required for the challenges
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, normalTexture.textureId);
			// DSA Version of two lines above
			//glBindTextureUnit(1, normalTexture.textureId);
			texturedCubeShader.uploadInt("uNormalTexture", 1);

			for (int i = 0; i < cubePositions.size(); i++)
			{
				drawCube(cubePositions[i], textures.at(cubeTextures.at(i)));
			}
		}
	}
}