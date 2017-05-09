#pragma once

#include <vector>
#include "glad/glad.h"

namespace Viper
{
	namespace Renderer
	{
		/** GLSL shader compiler utility class
		 *  This class has methods to compile GLSL shaders and link them
		 */
		class ShaderCompiler
		{
		public:
			/** Types of shaders supported by the class
			 */
			enum class ShaderType
			{
				VERTEX_SHADER,
				FRAGMENT_SHADER
			};

			/** Compiles a GLSL shader from its source string
			 *	@param shaderSource The string variable with source code of the shader
			 *	@param shaderType The type of the shader being compiled (Vertex shader / Fragment shader)
			 *	@return The id of the compiled shader
			 */
			static GLuint CompileShader(std::string shaderSource, ShaderType shaderType);
			/** Compiles a GLSL shader from a file
			*	@param filename The file containing the shader source code
			*	@param shaderType The type of the shader being compiled (Vertex shader / Fragment shader)
			*	@return The id of the compiled shader
			*/
			static GLuint CompileShaderFromFile(std::string filename, ShaderType shaderType);
			/** Creates a program by linking the given list of shaders
			*	@param shaders The list of shaders to link
			*	@return The id of the linked program
			*/
			static GLuint CreateProgramWithShaders(std::vector<GLuint>& shaders);
		};
	}
}
