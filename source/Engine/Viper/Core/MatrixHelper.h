#pragma once

#include "glm/glm.hpp"

namespace Viper
{
	class MatrixHelper final
	{
	public:
		MatrixHelper() = delete;
		MatrixHelper(const MatrixHelper& ) = delete;
		MatrixHelper& operator=(const MatrixHelper&) = delete;
		MatrixHelper(MatrixHelper&&) = delete;
		MatrixHelper& operator=(MatrixHelper&&) = delete;
		~MatrixHelper() = default;

		static void GetForward(glm::mat4& matrix, glm::vec3& vector);
		static void GetUp(glm::mat4& matrix, glm::vec3& vector);
		static void GetRight(glm::mat4& matrix, glm::vec3& vector);
		static void GetTranslation(glm::mat4& matrix, glm::vec3& vector);

		static void SetForward(glm::mat4& matrix, glm::vec3& forward);
		static void SetUp(glm::mat4& matrix, glm::vec3& up);
		static void SetRight(glm::mat4& matrix, glm::vec3& right);
		static void SetTranslation(glm::mat4& matrix, glm::vec3& translation);
	};
}