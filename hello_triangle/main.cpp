#include <glsl.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
using namespace std;

void keyCallback(GLFWwindow* window, int key, int, int action, int)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

#if defined(WIN32)
int WinMain(HINSTANCE /*hInst*/, HINSTANCE /*hPrevInst*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/)
#else
int main()
#endif
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto window = glfwCreateWindow(800, 600, "Hello Window", nullptr, nullptr);

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

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	array<glm::vec3, 3> vertices = {
		glm::vec3{-0.5f, -0.5f, 0.0f},
		glm::vec3{ 0.5f, -0.5f, 0.0f},
		glm::vec3{ 0.0f,  0.5f, 0.0f}
	};

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(0);

	glsl::Program prog{ { glsl::vertex_shader, "hello_triangle.vs"s },{ glsl::fragment_shader, "hello_triangle.fs"s } };
	prog.use();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);		
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}