#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <string>
#include <iostream>
using namespace std;
using namespace string_literals;

void keyCallback(GLFWwindow* window, int key, int, int action, int)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
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

	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);

	array<float, 9> vertices = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

	const auto vsSrc = R"(
#version 440 core
layout(location = 0) in vec3 aPos;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";
	const auto vs = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vs, 1, &vsSrc, nullptr);
	glCompileShader(vs);
	GLint success;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint logLen;
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &logLen);

		string erroLog(logLen, '\0');
		glGetShaderInfoLog(vs, logLen, nullptr, &erroLog[0]);
		cerr << "Unable to build vertex shader for this reason:\n" << erroLog << endl;
	}

	const auto fsSrc = R"(
#version 440 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
})";

	const auto fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fs, 1, &fsSrc, nullptr);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint logLen;
		glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &logLen);

		string erroLog(logLen, '\0');
		glGetShaderInfoLog(fs, logLen, nullptr, &erroLog[0]);
		cerr << "Unable to build fragment shader for this reason:\n" << erroLog << endl;
	}

	const auto prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog);

	glGetProgramiv(prog, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLint logLen;
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLen);

		string erroLog(logLen, '\0');
		glGetProgramInfoLog(prog, logLen, nullptr, &erroLog[0]);
		cerr << "Unable to link gpu program for this reason:\n" << erroLog << endl;
	}

	glUseProgram(prog);
	glDeleteShader(vs);
	glDeleteShader(fs);
	
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(0);

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