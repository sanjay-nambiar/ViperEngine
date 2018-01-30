#include "Pch.h"
#include "ShaderCompiler.h"
#include <fstream>
#include "Graphics/Shader.h"

#define INFO_LOG_LENGTH 512

namespace Viper
{
	namespace Graphics
	{

		Shader ShaderCompiler::CompileShader(std::string shaderSource, ShaderType shaderType)
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
			return {shaderId, shaderType};
		}

		Shader ShaderCompiler::CompileShaderFromFile(std::string filename, ShaderType shaderType)
		{
			// Read the shader source code from file
			std::ifstream file(filename);
			if (!file.is_open())
			{
				throw std::runtime_error("Unable to locate shader file");
			}
			std::string shaderSource((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			file.close();

			// Compile shader using the source code string read from the file
			return CompileShader(shaderSource, shaderType);
		}

		GLuint ShaderCompiler::CreateProgramWithShaders(const std::unordered_map<Graphics::ShaderType, Graphics::Shader>& shaders)
		{
			// Links all the shaders and creates a program
			GLuint shaderProgram = glCreateProgram();
			for (const auto& shader : shaders)
			{
				glAttachShader(shaderProgram, shader.second.Id());
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