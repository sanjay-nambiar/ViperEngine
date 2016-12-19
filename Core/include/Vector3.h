#ifndef VECTOR3_HEADER
#define VECTOR3_HEADER

#include "Types.h"

namespace Viper
{
	struct Vector3
	{
		float32_t x;
		float32_t y;
		float32_t z;

		inline Vector3(float32_t x, float32_t y, float32_t z) : x(x), y(y), z(z)
		{			
		}
	};
}


#endif // VECTOR3_HEADER