#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <string_view>
#include <initializer_list>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace glsl {
	
	enum ShaderType : GLenum {
		vertex_shader = GL_VERTEX_SHADER,
		fragment_shader = GL_FRAGMENT_SHADER
	};

	class Shader {
	public:
		Shader(ShaderType type, const std::string& filename)
			: mShader{ glCreateShader(type) }
		{
			using namespace std::string_literals;

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

				throw std::invalid_argument{filename + " failed to compile!\n"s + errorMessage };
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
			{
				glDeleteShader(mShader);
			}
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
		Program(std::initializer_list<Shader> shaders)
			: mProgram{ glCreateProgram() }
		{
			std::for_each(begin(shaders), end(shaders), [this](auto&& shader)
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
			{
				unuse();
				glDeleteProgram(mProgram);
			}
		}

		void use() noexcept
		{
			glUseProgram(mProgram);
		}

		void unuse() noexcept
		{
			glUseProgram(0);
		}

		void uniform(std::string str, GLuint val)
		{
			auto it = m_uniformMap.find(str);

			if (it == std::end(m_uniformMap))
				m_uniformMap.emplace(str, uniformLocation(str));

			glUniform1i(m_uniformMap[str], val);
		}

		void uniform(std::string str, const glm::mat4& mat)
		{
			auto it = m_uniformMap.find(str);

			if (it == std::end(m_uniformMap))
				m_uniformMap.emplace(str, uniformLocation(str));

			glUniformMatrix4fv(m_uniformMap[str], 1, GL_FALSE, glm::value_ptr(mat));
		}

	private:
		GLint uniformLocation(const std::string& str)
		{
			auto res = glGetUniformLocation(mProgram, str.c_str());

			if (res < 0)
				std::runtime_error{ "uniform not found" };

			return res;
		}

	private:
		GLuint mProgram;
		std::map<std::string, GLint> m_uniformMap;
	};

} // glsl