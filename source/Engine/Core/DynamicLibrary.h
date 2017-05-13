#pragma once

#include "Platform.h"

// Windows
#if VIPER_PLATFORM == VIPER_PLATFORM_WIN

	#define	DYNAMIC_LIB_HANDLE HINSTANCE
	#define	DYNAMIC_LIB_LOAD(libraryName) LoadLibraryEx(std::wstring(libraryName.begin(), libraryName.end()).c_str(), nullptr, 0)
	#define	DYNAMIC_LIB_GETSYM(libraryInstance, symbolName) GetProcAddress(libraryInstance, symbolName)
	#define	DYNAMIC_LIB_UNLOAD(libraryInstance) FreeLibrary(libraryInstance)
	
	#define MODULE_EXPORT __declspec(dllexport)
	#define MODULE_IMPORT __declspec(dllimport)

// Linux
#elif VIPER_PLATFORM == VIPER_PLATFORM_LINUX

	#define	DYNAMIC_LIB_HANDLE void*
	#define	DYNAMIC_LIB_LOAD(libraryName) dlopen(libraryName, RTLD_LAZY | RTLD_GLOBAL)
	#define	DYNAMIC_LIB_GETSYM(libraryInstance, symbolName) dlsym(libraryInstance, symbolName)
	#define	DYNAMIC_LIB_UNLOAD(libraryInstance) dlclose(libraryInstance)

	#define MODULE_EXPORT __attribute__((visibility("default")))
	#define MODULE_IMPORT

// Mac
#elif VIPER_PLATFORM == VIPER_PLATFORM_MAC

	#define	DYNAMIC_LIB_HANDLE void*
	#define	DYNAMIC_LIB_LOAD(libraryName) mac_loadDylib(libraryName)
	#define FRAMEWORK_LOAD(framework) mac_loadFramework(framework)
	#define	DYNAMIC_LIB_GETSYM(libraryInstance, symbolName) dlsym(libraryInstance, symbolName)
	#define	DYNAMIC_LIB_UNLOAD(libraryInstance) dlclose(libraryInstance)

	#define MODULE_EXPORT __attribute__((visibility("default")))
	#define MODULE_IMPORT

#endif
