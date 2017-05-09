#pragma once

#include "Service/AudioManager.h"


#define DllImport	__declspec(dllimport)


namespace Viper
{
	class ServiceLocator;
}

// Import declarations
extern "C" DllImport void ProvideAudio(uint32_t maxChannels, Viper::ServiceLocator& serviceLocator);
extern "C" DllImport void ProvideWindowSystem(Viper::ServiceLocator& serviceLocator);

// Function pointers
typedef void(__cdecl *funcProvideAudio) (uint32_t, Viper::ServiceLocator&);
typedef void(__cdecl *funcProvideWindowSystem) (Viper::ServiceLocator&);
