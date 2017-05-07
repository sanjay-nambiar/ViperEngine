#pragma once

#include "Service/AudioManager.h"


#define DllImport	__declspec(dllimport)


namespace Viper {
	class ServiceLocator;
}

// Import declarations
extern "C" DllImport void ProvideAudioManager(uint32_t maxChannels, Viper::ServiceLocator& serviceLocator);

// Function pointers
typedef void(__stdcall *funcProvideAudioManager) (uint32_t, Viper::ServiceLocator&);
