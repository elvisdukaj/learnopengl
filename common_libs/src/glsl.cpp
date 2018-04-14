#include <glsl.hpp>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace glsl {

using namespace std;
using namespace std::string_literals;

Shader::Shader(ShaderType type, const std::string& filename)
	: mShader{ glCreateShader(type) }
{
	std::ifstream file(filename);

	if (!file)
		throw std::invalid_argument{ "Unable to open: " + filename + " file!"s };

	std::stringstream buffer;
	buffer << file.rdbuf();

	auto srcData = buffer.str();
	auto ptr = srcData.c_str();
	glShaderSource(mShader, 1, &ptr, nullptr);

	glCompileShader(mShader);

	GLint success;
	glGetShaderiv(mShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		using namespace std::string_literals;
		GLsizei infoLogLen;
		glGetShaderiv(mShader, GL_INFO_LOG_LENGTH, &infoLogLen);

		std::string errorMessage(infoLogLen, '\0');
		glGetShaderInfoLog(mShader, infoLogLen, nullptr, &errorMessage[0]);

		throw std::invalid_argument{ filename + " failed to compile!\n"s + errorMessage };
	}
}

Shader::Shader(Shader&& rhs) : mShader{ rhs.mShader }
{
	rhs.mShader = 0;
}

Shader& Shader::operator = (Shader&& rhs)
{
	mShader = rhs.mShader;
	rhs.mShader = 0;
	return *this;
}

Shader::~Shader()
{
	if (mShader)
	{
		glDeleteShader(mShader);
	}
}

Program::Program(std::initializer_list<Shader> shaders)
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

		std::string errorMessage(infoLogLen, '\0');
		glGetProgramInfoLog(mProgram, infoLogLen, nullptr, &errorMessage[0]);

		throw std::invalid_argument{ errorMessage };
	}

	glValidateProgram(mProgram);
	glGetProgramiv(mProgram, GL_VALIDATE_STATUS, &success);
	if (!success)
	{
		GLsizei infoLogLen;
		glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &infoLogLen);

		std::string errorMessage(infoLogLen, '\0');
		glGetProgramInfoLog(mProgram, infoLogLen, nullptr, &errorMessage[0]);

		throw std::invalid_argument{ errorMessage };
	}
}

Program::Program(Program&& rhs)
	: mProgram(rhs.mProgram)
{
	rhs.mProgram = 0;
}

Program& Program::operator = (Program&& rhs)
{
	mProgram = rhs.mProgram;
	return *this;
}

Program::~Program()
{
	if (mProgram)
	{
		unuse();
		glDeleteProgram(mProgram);
	}
}


GLint Program::uniformLocation(const std::string& str)
{
	auto res = glGetUniformLocation(mProgram, str.c_str());

	if (res < 0)
		std::runtime_error{ "uniform not found" };

	return res;
}
}