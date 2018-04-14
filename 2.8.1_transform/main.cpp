#define GLM_FORCE_MESSAGES

#include "stb_image.h"
#include <glsl.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
using namespace std;

static int g_width = 800, g_height = 600;

void keyCallback(GLFWwindow* window, int key, int, int action, int)
{
	static bool wireframe = false;

	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, true);
		break;

	case GLFW_KEY_W:
		if (action != GLFW_PRESS)
			break;

		wireframe = !wireframe;

		glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
		
	default:
		break;
	}
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	g_width = width;
	g_height = height;
	glViewport(0, 0, width, height);
}

#if defined(WIN32)
int wWinMain(HINSTANCE /*hInst*/, HINSTANCE /*hPrevInst*/, LPWSTR /*lpCmdLine*/, int /*nCmdShow*/)
#else
int main()
#endif
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto window = glfwCreateWindow(g_width, g_height, "2.8.1 Transform", nullptr, nullptr);

	if (!window)
	{
		const char* error;
		glfwGetError(&error);
		cerr << "Unable to open the window: " << error << endl;
		glfwTerminate();

		return 1;
	}

	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
	glfwSetKeyCallback(window, keyCallback);

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSwapInterval(1);
	glClearColor(0.2f, 0.3f, 3.0f, 0.0f);
;
	array<glm::vec3, 4> vertices = {
		glm::vec3{ 0.5f,  0.5f, 0.0f}, // top right
		glm::vec3{ 0.5f, -0.5f, 0.0f}, // bottom right
    	glm::vec3{-0.5f, -0.5f, 0.0f}, // bottom left
		glm::vec3{-0.5f,  0.5f, 0.0f}, // top left
	};

	array<glm::vec3, 4> colors = {
		glm::vec3{ 1.0f, 0.0f, 0.0f }, // top right
		glm::vec3{ 0.0f, 1.0f, 0.0f }, // bottom right
		glm::vec3{ 0.0f, 0.0f, 1.0f }, // bottom left
		glm::vec3{ 1.0f, 1.0f, 0.0f }, // top left
	};

	array<glm::vec2, 4> texCoords = {
		glm::vec2{1.0f, 1.0f},         // top right
		glm::vec2{1.0f, 0.0f},         // bottom right
		glm::vec2{0.0f, 0.0f},         // bottom left
		glm::vec2{0.0f, 1.0f},         // top left
	};

	array<GLuint, 6> indexes{ 0, 1, 2, 2, 3, 0 };

	GLuint idx_ebo;
	glGenBuffers(1, &idx_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes.data(), GL_STATIC_DRAW);

	GLuint pos_vbo;
	glGenBuffers(1, &pos_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

	GLuint col_vbo;
	glGenBuffers(1, &col_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, col_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors.data(), GL_STATIC_DRAW);

	GLuint tex_vbo;
	glGenBuffers(1, &tex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, tex_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords.data(), GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(GLfloat), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, col_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 3 * sizeof(GLfloat), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, tex_vbo);
	glVertexAttribPointer(2, 3, GL_FLOAT, false, 2 * sizeof(GLfloat), nullptr);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_ebo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	
	array<GLuint, 2> textures;
	glGenTextures((GLsizei)textures.size(), textures.data());
	
	int width, height, channels;
	auto rawWallImageData = stbi_load("resources/textures/wall.jpeg", &width, &height, &channels, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (rawWallImageData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rawWallImageData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(rawWallImageData);

	stbi_set_flip_vertically_on_load(true);
	auto rawSmileImageData = stbi_load("resources/textures/awesomeface.png", &width, &height, &channels, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (rawSmileImageData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rawSmileImageData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(rawSmileImageData);
	
	glsl::Program prog{
		{ glsl::vertex_shader  , "resources/shaders/2.8.1_transform.vs"s },
		{ glsl::fragment_shader, "resources/shaders/2.8.1_transform.fs"s }
	};
	
	prog.use();
	prog.uniform("text1"s, 0);
	prog.uniform("text2"s, 1);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		auto model = glm::mat4{ 1.0f };
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3{ 1.0f, 0.0f, 0.0f });

		auto view = glm::mat4{ 1.0f };
		view = glm::translate(view, glm::vec3{ 0.0f, 0.0f, -1.0f });

		auto projection = glm::perspective(glm::degrees(45.0f), static_cast<float>(g_width) / g_height, 0.1f, 100.0f);

		prog.uniform("model"s, model);
		prog.uniform("view"s, view);
		prog.uniform("projection"s, projection);

		glDrawElements(GL_TRIANGLES, (GLuint)indexes.size(), GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);		
	}

	glDeleteBuffers(1, &pos_vbo);
	glDeleteBuffers(1, &col_vbo);
	glDeleteVertexArrays(1, &vao);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}