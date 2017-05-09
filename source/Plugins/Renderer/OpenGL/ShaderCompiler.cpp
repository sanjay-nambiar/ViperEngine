#include "Pch.h"
#include "ShaderCompiler.h"
#include <fstream>

#define INFO_LOG_LENGTH 512

namespace Viper
{
	namespace Renderer
	{
		GLuint ShaderCompiler::CompileShader(std::string shaderSource, ShaderType shaderType)
		{
			GLuint shaderId;
			if (shaderType == ShaderType::VERTEX_SHADER)
			{
				shaderId = glCreateShader(GL_VERTEX_SHADER);
			}
			else
			{
				shaderId = glCreateShader(GL_FRAGMENT_SHADER);
			}

			// Set shader source code and compile
			const char* shaderSourcePtr = shaderSource.c_str();
			glShaderSource(shaderId, 1, &shaderSourcePtr, nullptr);
			glCompileShader(shaderId);

			// Check if the shader has compiled successfully. Throw exception otherwise
			GLint success;
			char infoLog[INFO_LOG_LENGTH];
			glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shaderId, INFO_LOG_LENGTH, nullptr, infoLog);
				throw std::runtime_error(infoLog);
			}
			return shaderId;
		}

		GLuint ShaderCompiler::CompileShaderFromFile(std::string filename, ShaderType shaderType)
		{
			// Read the shader source code from file
			std::ifstream file(filename);
			if (!file.is_open())
			{
				throw std::runtime_error("Unable to locate shader file");
			}
			std::string shaderSource;
			file.seekg(0, std::ios::end);
			shaderSource.reserve(static_cast<int>(file.tellg()));
			file.seekg(0, std::ios::beg);
			shaderSource.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			file.close();

			// Compile shader using the source code string read from the file
			return CompileShader(shaderSource, shaderType);
		}

		GLuint ShaderCompiler::CreateProgramWithShaders(std::vector<GLuint>& shaders)
		{
			// Links all the shaders and creates a program
			GLuint shaderProgram = glCreateProgram();
			for (GLuint shader : shaders)
			{
				glAttachShader(shaderProgram, shader);
			}
			glLinkProgram(shaderProgram);

			// Check if the linking was successfull. Throw exception if linking failed.
			GLint success;
			char infoLog[INFO_LOG_LENGTH];
			glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shaderProgram, INFO_LOG_LENGTH, nullptr, infoLog);
				throw std::runtime_error(infoLog);
			}
			return shaderProgram;
		}
	}
}