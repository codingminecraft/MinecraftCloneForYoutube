#include "core.h"
#include "core/Window.h"
#include "core/Input.h"
#include "renderer/Shader.h"

using namespace MinecraftClone;

const int windowWidth = 1920;
const int windowHeight = 1080;
const char* windowTitle = "OpenGL Template";

static Shader shader;

int exitWithError(const char* msg)
{
	printf(msg);
	glfwTerminate();
	return -1;
}

struct Vertex
{
	std::array<float, 4> color;
	std::array<float, 3> position;
};

// =============================================================
// Challenge 1 Solution
// =============================================================
static std::array<Vertex, 6> challenge1Square = {
	//      Color                         Position
	Vertex{{0.9f,  0.1f, 0.12f, 1.0f},   {-0.5f, -0.5f, 0.0f}},
	Vertex{{0.1f,  0.9f, 0.12f, 1.0f},   {-0.5f,  0.5f, 0.0f}},
	Vertex{{0.12f, 0.9f, 0.1f,  1.0f},   { 0.5f,  0.5f, 0.0f}},

	Vertex{{0.9f,  0.1f, 0.12f, 1.0f},   {-0.5f, -0.5f, 0.0f}},
	Vertex{{0.12f, 0.9f, 0.1f,  1.0f},   { 0.5f,  0.5f, 0.0f}},
	Vertex{{0.12f, 0.1f, 0.9f,  1.0f},   { 0.5f, -0.5f, 0.0f}}
};

static uint32 challenge1Vao;
static uint32 challenge1Vbo;

void setupChallenge1()
{
	glCreateVertexArrays(1, &challenge1Vao);
	glBindVertexArray(challenge1Vao);

	glGenBuffers(1, &challenge1Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, challenge1Vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(challenge1Square), challenge1Square.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
}

void drawChallenge1()
{	
	glBindVertexArray(challenge1Vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void destroyChallenge1()
{
	glDeleteBuffers(1, &challenge1Vbo);
	glDeleteVertexArrays(1, &challenge1Vao);
}

// =============================================================
// Challenge 2 Solution
// =============================================================
static std::array<Vertex, 4> challenge2Square = {
	//      Color                         Position
	Vertex{{0.11f, 0.8f, 0.76f, 1.0f},   {-0.5f, -0.5f, 0.0f}},
	Vertex{{0.1f,  0.9f, 0.12f, 1.0f},   {-0.5f,  0.5f, 0.0f}},
	Vertex{{0.12f, 0.9f, 0.1f,  1.0f},   { 0.5f,  0.5f, 0.0f}},
	Vertex{{0.12f, 0.1f, 0.9f,  1.0f},   { 0.5f, -0.5f, 0.0f}}
};

static uint32 challenge2Elements[6] = {
	0, 1, 2,
	0, 2, 3
};

static uint32 challenge2Vao;
static uint32 challenge2Vbo;
static uint32 challenge2Ebo;

void setupChallenge2()
{
	glCreateVertexArrays(1, &challenge2Vao);
	glBindVertexArray(challenge2Vao);

	glGenBuffers(1, &challenge2Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, challenge2Vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(challenge2Square), challenge2Square.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &challenge2Ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, challenge2Ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(challenge2Elements), challenge2Elements, GL_STATIC_DRAW);
}

void drawChallenge2()
{
	glBindVertexArray(challenge2Vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void destroyChallenge2()
{
	glDeleteBuffers(1, &challenge2Vbo);
	glDeleteBuffers(1, &challenge2Ebo);
	glDeleteVertexArrays(1, &challenge2Vao);
}

// =============================================================
// Challenge 3 Solution
// =============================================================
static std::array<Vertex, 10> challenge3Star = {
	//      Color                         Position
	Vertex{{  0.4f, 0.521f, 0.960f, 1.0f},   {  -0.4f,  0.125f, 0.0f}},
	Vertex{{0.490f, 0.443f, 0.956f, 1.0f},   {-0.125f,  0.125f, 0.0f}},
	Vertex{{0.686f, 0.443f, 0.956f, 1.0f},   {   0.0f,    0.5f, 0.0f}},
	Vertex{{0.917f, 0.443f, 0.956f, 1.0f},   { 0.125f,  0.125f, 0.0f}},
	Vertex{{0.807f, 0.317f, 0.250f, 1.0f},   {   0.4f,  0.125f, 0.0f}},
	Vertex{{0.807f, 0.250f, 0.682f, 1.0f},   {  0.13f, -0.125f, 0.0f}},
	Vertex{{0.956f, 0.631f, 0.443f, 1.0f},   {  0.29f,   -0.6f, 0.0f}},
	Vertex{{0.956f, 0.843f, 0.443f, 1.0f},   {   0.0f,  -0.29f, 0.0f}},
	Vertex{{0.862f, 0.956f, 0.443f, 1.0f},   { -0.29f,   -0.6f, 0.0f}},
	Vertex{{0.584f, 0.956f, 0.443f, 1.0f},   { -0.13f, -0.125f, 0.0f}}
};

static uint32 challenge3Elements[24] = {
	0, 1, 9,   1, 2, 3,
	3, 4, 5,   5, 6, 7,
	7, 8, 9,   9, 5, 7,
	9, 1, 3,   9, 3, 5
};

static uint32 challenge3Vao;
static uint32 challenge3Vbo;
static uint32 challenge3Ebo;

void setupChallenge3()
{
	glCreateVertexArrays(1, &challenge3Vao);
	glBindVertexArray(challenge3Vao);

	glGenBuffers(1, &challenge3Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, challenge3Vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(challenge3Star), challenge3Star.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &challenge3Ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, challenge3Ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(challenge3Elements), challenge3Elements, GL_STATIC_DRAW);
}

void drawChallenge3()
{
	glBindVertexArray(challenge3Vao);
	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
}

void destroyChallenge3()
{
	glDeleteBuffers(1, &challenge3Vbo);
	glDeleteBuffers(1, &challenge3Ebo);
	glDeleteVertexArrays(1, &challenge3Vao);
}

// =============================================================
// Challenge 4 Solution
// =============================================================
static std::array<Vertex, 4> challenge4Outline = {
	//      Color                         Position
	//      Color                         Position
	Vertex{{1.0f, 1.0f, 1.0f, 1.0f},   {-0.5f, -0.5f, 0.0f}},
	Vertex{{1.0f, 1.0f, 1.0f, 1.0f},   {-0.5f,  0.5f, 0.0f}},
	Vertex{{1.0f, 1.0f, 1.0f, 1.0f},   { 0.5f,  0.5f, 0.0f}},
	Vertex{{1.0f, 1.0f, 1.0f, 1.0f},   { 0.5f, -0.5f, 0.0f}}
};

// NOTE: I only use 2 elements per primitive since we
// are drawing lines here and not triangles
static uint32 challenge4Elements[8] = {
	0, 1,    1, 2,
	2, 3,    3, 0
};

static uint32 challenge4Vao;
static uint32 challenge4Vbo;
static uint32 challenge4Ebo;

void setupChallenge4()
{
	glCreateVertexArrays(1, &challenge4Vao);
	glBindVertexArray(challenge4Vao);

	// Set up and buffer element buffer
	glCreateBuffers(1, &challenge4Ebo);
	glNamedBufferData(challenge4Ebo, sizeof(challenge4Elements), challenge4Elements, GL_STATIC_DRAW);
	glVertexArrayElementBuffer(challenge4Vao, challenge4Ebo);

	// Set up and buffer the vertex buffer
	glCreateBuffers(1, &challenge4Vbo);
	glNamedBufferData(challenge4Vbo, sizeof(challenge4Outline), challenge4Outline.data(), GL_STATIC_DRAW);
	glVertexArrayVertexBuffer(challenge4Vao, 0, challenge4Vbo, 0, sizeof(Vertex));

	// Set up and enable attribute 1
	glVertexArrayAttribFormat(challenge4Vao, 0, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, color));
	glVertexArrayAttribBinding(challenge4Vao, 0, 0);
	glEnableVertexArrayAttrib(challenge4Vao, 0);

	// Set up and enable attribute 2
	glVertexArrayAttribFormat(challenge4Vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
	glVertexArrayAttribBinding(challenge4Vao, 1, 0);
	glEnableVertexArrayAttrib(challenge4Vao, 1);
}

void drawChallenge4()
{
	glBindVertexArray(challenge4Vao);
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);
}

void destroyChallenge4()
{
	glDeleteBuffers(1, &challenge4Vbo);
	glDeleteBuffers(1, &challenge4Ebo);
	glDeleteVertexArrays(1, &challenge4Vao);
}

// =============================================================
// Challenge 5 Solution
// =============================================================
static std::array<Vertex, 10> challenge5Star = {
	//      Color                         Position
	Vertex{{0.133f, 0.549f, 0.141f, 1.0f},   {  -0.4f,  0.125f, 0.0f}},
	Vertex{{0.929f, 0.968f, 0.031f, 1.0f},   {-0.125f,  0.125f, 0.0f}},
	Vertex{{0.929f, 0.968f, 0.031f, 1.0f},   {   0.0f,    0.5f, 0.0f}},
	Vertex{{0.968f, 0.031f, 0.133f, 1.0f},   { 0.125f,  0.125f, 0.0f}},
	Vertex{{0.968f, 0.031f, 0.133f, 1.0f},   {   0.4f,  0.125f, 0.0f}},
	Vertex{{0.945f, 0.031f, 0.968f, 1.0f},   {  0.13f, -0.125f, 0.0f}},
	Vertex{{0.945f, 0.031f, 0.968f, 1.0f},   {  0.29f,   -0.6f, 0.0f}},
	Vertex{{0.619f, 0.031f, 0.968f, 1.0f},   {   0.0f,  -0.29f, 0.0f}},
	Vertex{{0.619f, 0.031f, 0.968f, 1.0f},   { -0.29f,   -0.6f, 0.0f}},
	Vertex{{0.619f, 0.031f, 0.968f, 1.0f},   { -0.13f, -0.125f, 0.0f}}
};

static uint32 challenge5Elements[24] = {
	0, 1, 9,   1, 2, 3,
	3, 4, 5,   5, 6, 7,
	7, 8, 9,   9, 5, 7,
	9, 1, 3,   9, 3, 5
};

static uint32 challenge5Vao;
static uint32 challenge5Vbo;
static uint32 challenge5Ebo;

void setupChallenge5()
{
	glCreateVertexArrays(1, &challenge5Vao);
	glBindVertexArray(challenge5Vao);

	// Set up and buffer element buffer
	glCreateBuffers(1, &challenge5Ebo);
	glNamedBufferData(challenge5Ebo, sizeof(challenge5Elements), challenge5Elements, GL_STATIC_DRAW);
	glVertexArrayElementBuffer(challenge5Vao, challenge5Ebo);

	// Set up and buffer the vertex buffer
	glCreateBuffers(1, &challenge5Vbo);
	glNamedBufferData(challenge5Vbo, sizeof(challenge5Star), challenge5Star.data(), GL_STATIC_DRAW);
	glVertexArrayVertexBuffer(challenge5Vao, 0, challenge5Vbo, 0, sizeof(Vertex));

	// Set up and enable attribute 1
	glVertexArrayAttribFormat(challenge5Vao, 0, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, color));
	glVertexArrayAttribBinding(challenge5Vao, 0, 0);
	glEnableVertexArrayAttrib(challenge5Vao, 0);

	// Set up and enable attribute 2
	glVertexArrayAttribFormat(challenge5Vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
	glVertexArrayAttribBinding(challenge5Vao, 1, 0);
	glEnableVertexArrayAttrib(challenge5Vao, 1);
}

void drawChallenge5()
{
	glBindVertexArray(challenge5Vao);
	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
}

void destroyChallenge5()
{
	glDeleteBuffers(1, &challenge5Vbo);
	glDeleteBuffers(1, &challenge5Ebo);
	glDeleteVertexArrays(1, &challenge5Vao);
}

enum class ChallengeType : uint8
{
	None = 0,
	Challenge1,
	Challenge2,
	Challenge3,
	Challenge4,
	Challenge5
};

static ChallengeType currentChallenge = ChallengeType::None;

void destroyCurrentChallenge()
{
	switch (currentChallenge)
	{
	case ChallengeType::Challenge1:
		destroyChallenge1();
		break;
	case ChallengeType::Challenge2:
		destroyChallenge2();
		break;
	case ChallengeType::Challenge3:
		destroyChallenge3();
		break;
	case ChallengeType::Challenge4:
		destroyChallenge4();
		break;
	case ChallengeType::Challenge5:
		destroyChallenge5();
		break;
	default:
		break;
	}
}

int main()
{
	glfwInit();

	Window* window = Window::createWindow(windowWidth, windowHeight, windowTitle);
	if (window == nullptr)
	{
		return exitWithError("Failed to create GLFW window\n");
	}
	window->installMainCallbacks();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return exitWithError("Failed to initialize GLAD.\n");
	}

	// Make sure not to call any OpenGL functions until *after* we initialize our function loader
	shader.compile();

	glViewport(0, 0, windowWidth, windowHeight);
	shader.bind();
	while (!glfwWindowShouldClose(window->nativeWindow))
	{
		glClearColor(0.266f, 0.466f, 0.698f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Check if the user pressed any of the keys 1-6
		// If they did delete the challenge we were on and setup up the next challenge
		if (Input::isKeyDown(GLFW_KEY_1))
		{
			if (currentChallenge != ChallengeType::Challenge1)
			{
				destroyCurrentChallenge();
				setupChallenge1();
				currentChallenge = ChallengeType::Challenge1;
			}
		}
		else if (Input::isKeyDown(GLFW_KEY_2))
		{
			if (currentChallenge != ChallengeType::Challenge2)
			{
				destroyCurrentChallenge();
				setupChallenge2();
				currentChallenge = ChallengeType::Challenge2;
			}
		}
		else if (Input::isKeyDown(GLFW_KEY_3))
		{
			if (currentChallenge != ChallengeType::Challenge3)
			{
				destroyCurrentChallenge();
				setupChallenge3();
				currentChallenge = ChallengeType::Challenge3;
			}
		}
		else if (Input::isKeyDown(GLFW_KEY_4))
		{
			if (currentChallenge != ChallengeType::Challenge4)
			{
				destroyCurrentChallenge();
				setupChallenge4();
				currentChallenge = ChallengeType::Challenge4;
			}
		}
		else if (Input::isKeyDown(GLFW_KEY_5))
		{
			if (currentChallenge != ChallengeType::Challenge5)
			{
				destroyCurrentChallenge();
				setupChallenge5();
				currentChallenge = ChallengeType::Challenge5;
			}
		}

		// Draw whichever challenge we are currently displaying
		switch (currentChallenge)
		{
		case ChallengeType::Challenge1:
			drawChallenge1();
			break;
		case ChallengeType::Challenge2:
			drawChallenge2();
			break;
		case ChallengeType::Challenge3:
			drawChallenge3();
			break;
		case ChallengeType::Challenge4:
			drawChallenge4();
			break;
		case ChallengeType::Challenge5:
			drawChallenge5();
			break;
		default:
			break;
		}

		glfwSwapBuffers(window->nativeWindow);
		glfwPollEvents();
	}

	Window::freeWindow(window);
	glfwTerminate();
	return 0;
}

