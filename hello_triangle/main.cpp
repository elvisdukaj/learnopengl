#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <vector>
#include <string>
#include <string_view>
#include <iostream>
#include <stdexcept>
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

namespace glsl{

enum ShaderType : GLenum {
	vertex = GL_VERTEX_SHADER,
	fragment = GL_FRAGMENT_SHADER
};


class Shader {
public:
	Shader(ShaderType type, const string_view& src)
		: mShader{ glCreateShader(type) }
	{
		auto srcData = src.data();
		glShaderSource(mShader, 1, &srcData, nullptr);

		glCompileShader(mShader);

		GLint success;
		glGetShaderiv(mShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			GLsizei infoLogLen;
			glGetShaderiv(mShader, GL_INFO_LOG_LENGTH, &infoLogLen);

			string errorMessage(infoLogLen, '\0');
			glGetShaderInfoLog(mShader, infoLogLen, nullptr, &errorMessage[0]);

			throw invalid_argument{ errorMessage };
		}
	}

	Shader(Shader&) = delete;
	Shader& operator = (const Shader&) = delete;

	Shader(Shader&& rhs)
		: mShader{ rhs.mShader }
	{
		rhs.mShader = 0;
	}

	Shader& operator = (Shader&& rhs)
	{
		mShader = rhs.mShader;
		rhs.mShader = 0;
		return *this;
	}

	~Shader()
	{
		if (mShader)
			glDeleteShader(mShader);
	}

	operator GLuint() const noexcept
	{
		return mShader;
	}

private:
	GLuint mShader;
};

class Program {
public:
	Program(initializer_list<Shader> shaders)
		: mProgram{ glCreateProgram() }
	{
		for_each(begin(shaders), end(shaders), [this](auto&& shader)
		{
			glAttachShader(mProgram, shader);
		});

		GLint success;

		glLinkProgram(mProgram);
		glGetProgramiv(mProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLsizei infoLogLen;
			glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &infoLogLen);

			string errorMessage(infoLogLen, '\0');
			glGetProgramInfoLog(mProgram, infoLogLen, nullptr, &errorMessage[0]);

			throw invalid_argument{ errorMessage };
		}

		glValidateProgram(mProgram);
		glGetProgramiv(mProgram, GL_VALIDATE_STATUS, &success);
		if (!success)
		{
			GLsizei infoLogLen;
			glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &infoLogLen);

			string errorMessage(infoLogLen, '\0');
			glGetProgramInfoLog(mProgram, infoLogLen, nullptr, &errorMessage[0]);

			throw invalid_argument{ errorMessage };
		}
	}

	explicit Program(Program&& rhs)
		: mProgram(rhs.mProgram)
	{
		rhs.mProgram = 0;
	}

	Program(const Program&) = delete;
	Program& operator = (const Program&) = delete;

	Program& operator = (Program&& rhs)
	{
		mProgram = rhs.mProgram;
		return *this;
	}

	~Program()
	{
		if (mProgram)
			glDeleteProgram(mProgram);
	}

	void use() noexcept
	{
		glUseProgram(mProgram);
	}

	void unuse() noexcept
	{
		glUseProgram(0);
	}

private:
	GLuint mProgram;
};

} // glsl

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

	const auto fsSrc = R"(
#version 440 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
})";

	glsl::Program prog{ {glsl::vertex, vsSrc}, {glsl::fragment, fsSrc} };
	
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(0);

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