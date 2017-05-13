#pragma once

/** Supported platforms are Windows, Linux and Mac
 *	Expected to be compiled with MSVC in windows, G++ in Linux and Clang/G++ in Mac
 */

#define VIPER_PLATFORM_WIN		1
#define VIPER_PLATFORM_LINUX	2
#define VIPER_PLATFORM_MAC		3


#if defined(_WIN32)
	#define VIPER_PLATFORM	VIPER_PLATFORM_WIN
	#include "Windows.h"
#elif defined(__linux__)
	#define VIPER_PLATFORM	VIPER_PLATFORM_LINUX
#elif defined(__APPLE__ && __MACH__)
	#define VIPER_PLATFORM	VIPER_PLATFORM_MAC
#endif
