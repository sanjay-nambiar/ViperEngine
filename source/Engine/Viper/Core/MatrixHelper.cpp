#include "Pch.h"
#include "MatrixHelper.h"

using namespace glm;

namespace Viper
{
	void MatrixHelper::GetForward(mat4& matrix, vec3& vector)
	{
		vec4 m3 = matrix[2];

        vector.x = -m3.x;
        vector.y = -m3.y;
        vector.z = -m3.z;
	}

	void MatrixHelper::GetUp(mat4& matrix, vec3& vector)
	{
		vec4 m2 = matrix[1];

		vector.x = m2.x;
        vector.y = m2.y;
        vector.z = m2.z;
	}

	void MatrixHelper::GetRight(mat4& matrix, vec3& vector)
	{
		vec4 m1 = matrix[0];

		vector.x = m1.x;
        vector.y = m1.y;
        vector.z = m1.z;
	}

	void MatrixHelper::GetTranslation(mat4& matrix, vec3& vector)
	{
		vec4 m4 = matrix[3];
		
		vector.x = m4.x;
        vector.y = m4.y;
        vector.z = m4.z;
	}

	void MatrixHelper::SetForward(mat4& matrix, vec3& forward)
	{
		vec4 m3 = matrix[2];

		m3.x = -forward.x;
        m3.y = -forward.y;
        m3.z = -forward.z;

		matrix[2] = m3;
	}

	void MatrixHelper::SetUp(mat4& matrix, vec3& up)
	{
		vec4 m2 = matrix[1];

		m2.x = up.x;
        m2.y = up.y;
        m2.z = up.z;

		matrix[1] = m2;
	}

	void MatrixHelper::SetRight(mat4& matrix, vec3& right)
	{
		vec4 m1 = matrix[0];

		m1.x = right.x;
        m1.y = right.y;
        m1.z = right.z;

		matrix[0] = m1;
	}

	void MatrixHelper::SetTranslation(mat4& matrix, vec3& translation)
	{
		vec4 m4 = matrix[3];

		m4.x = translation.x;
        m4.y = translation.y;
        m4.z = translation.z;

		matrix[3] = m4;
	}
}