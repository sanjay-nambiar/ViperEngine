#pragma once

#include "Service/AudioManager.h"
#include "DynamicLibrary.h"

namespace Viper
{
	class ServiceLocator;
}

// Import declarations
extern "C" MODULE_IMPORT void ProvideAudio(uint32_t maxChannels, Viper::ServiceLocator& serviceLocator);
extern "C" MODULE_IMPORT void ProvideWindowSystem(Viper::ServiceLocator& serviceLocator);
extern "C" MODULE_IMPORT void ProvideRenderer(Viper::ServiceLocator& serviceLocator);

// Function pointers
typedef void(__cdecl *funcProvideAudio) (uint32_t, Viper::ServiceLocator&);
typedef void(__cdecl *funcProvideWindowSystem) (Viper::ServiceLocator&);
typedef void(__cdecl *funcProvideRenderer) (Viper::ServiceLocator&);
