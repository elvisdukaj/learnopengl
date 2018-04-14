#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <initializer_list>
#include <string>
#include <map>

namespace glsl {
	
	enum ShaderType : GLenum {
		vertex_shader = GL_VERTEX_SHADER,
		fragment_shader = GL_FRAGMENT_SHADER
	};

	class Shader {
	public:
		Shader(ShaderType type, const std::string& filename);
		Shader(Shader&& rhs);
		Shader(Shader&) = delete;

		Shader& operator = (const Shader&) = delete;
		Shader& operator = (Shader&& rhs);

		~Shader();		

		operator GLuint() const noexcept
		{
			return mShader;
		}

	private:
		GLuint mShader;
	};

	class Program {
	public:
		Program(std::initializer_list<Shader> shaders);
		Program(Program&& rhs);
		Program(const Program&) = delete;
		
		Program& operator = (const Program&) = delete;
		Program& operator = (Program&& rhs);
		
		~Program();

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
			m_uniformMap.insert_or_assign(str, uniformLocation(str));
			glUniform1i(m_uniformMap[str], val);
		}

		void uniform(std::string str, const glm::mat4& mat)
		{
			m_uniformMap.insert_or_assign(str, uniformLocation(str));
			glUniformMatrix4fv(m_uniformMap[str], 1, GL_FALSE, glm::value_ptr(mat));
		}

	private:
		GLint uniformLocation(const std::string& str);

	private:
		GLuint mProgram;
		std::map<std::string, GLint> m_uniformMap;
	};

} // glsl