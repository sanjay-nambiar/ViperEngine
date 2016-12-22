#ifndef VECTOR3_HEADER
#define VECTOR3_HEADER

#include "Types.h"

namespace Viper
{
	/** 3D Vector class
	*  This class is used across the Viper Engine for all 3D vector math use case 
	*/
	struct Vector3
	{
		/**
		 * X component of the 3D vector
		 */
		float32_t x;
		/**
		* Y component of the 3D vector
		*/
		float32_t y;
		/**
		* Z component of the 3D vector
		*/
		float32_t z;

		/** Construct a 3D vector from x, y and z components
		 * @param x X component of the vector
		 * @param y Y component of the vector
		 * @param z Z component of the vector
		*/
		inline Vector3(float32_t x, float32_t y, float32_t z) : x(x), y(y), z(z)
		{			
		}
	};
}


#endif // VECTOR3_HEADER