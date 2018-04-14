#include <glsl.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
using namespace std;

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

	glfwSwapInterval(1);
	glClearColor(0.2f, 0.3f, 3.0f, 0.0f);
;

	array<glm::vec3, 3> vertices = {
		glm::vec3{ 0.5f,  0.5f, 0.0f},  // top right
		glm::vec3{ 0.5f, -0.5f, 0.0f},  // bottom right
    	glm::vec3{-0.5f, -0.5f, 0.0f},  // bottom left
	};

	array<glm::vec3, 3> colors = {
		glm::vec3{ 1.0f, 0.0f, 0.0f },  // red
		glm::vec3{ 0.0f, 1.0f, 0.0f },  // green
		glm::vec3{ 0.0f, 0.0f, 1.0f },  // blue
	};

	GLuint pos_vbo;
	glGenBuffers(1, &pos_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

	GLuint col_vbo;
	glGenBuffers(1, &col_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, col_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors.data(), GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(GLfloat), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, col_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 3 * sizeof(GLfloat), nullptr);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glsl::Program prog{ { glsl::vertex_shader, "hello_triangle2.vs"s },{ glsl::fragment_shader, "hello_triangle2.fs"s } };
	prog.use();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());

		glfwSwapBuffers(window);		
	}

	glDeleteBuffers(1, &pos_vbo);
	glDeleteBuffers(1, &col_vbo);
	glDeleteVertexArrays(1, &vao);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}