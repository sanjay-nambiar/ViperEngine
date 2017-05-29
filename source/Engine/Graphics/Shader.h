#pragma once

#include <cstdint>

namespace Viper
{
	namespace Graphics
	{
		/** Types of shaders supported by the class
		*/
		enum class ShaderType
		{
			VERTEX_SHADER,
			FRAGMENT_SHADER,
			GEOMETRY_SHADER,
			TESSELLATION_SHADER,
			PRIMITIVE_SHADER,
			COMPUTE_SHADER
		};

		class Shader final
		{
		public:
			Shader(std::uint32_t shaderId, ShaderType shaderType);
			~Shader() = default;

			std::uint32_t Id() const;
			ShaderType Type() const;
		private:
			std::uint32_t id;
			ShaderType type;
		};
	}
}
