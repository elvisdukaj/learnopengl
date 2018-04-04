#pragma once

#include <glad/glad.h>
#include <algorithm>
#include <string_view>
#include <initializer_list>
#include <string>
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
			std::ifstream file(filename);
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
				GLsizei infoLogLen;
				glGetShaderiv(mShader, GL_INFO_LOG_LENGTH, &infoLogLen);

				std::string errorMessage(infoLogLen, '\0');
				glGetShaderInfoLog(mShader, infoLogLen, nullptr, &errorMessage[0]);

				throw std::invalid_argument{ errorMessage };
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

	private:
		GLuint mProgram;
	};

} // glsl