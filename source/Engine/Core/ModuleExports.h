#pragma once

#include "Service/AudioManager.h"


#define DllExport	__declspec(dllexport)


namespace Viper
{
	class ServiceLocator;
}

// All module initialization export declarations
extern "C" DllExport void ProvideAudio(uint32_t maxChannels, Viper::ServiceLocator& serviceLocator);
extern "C" DllExport void ProvideWindowSystem(Viper::ServiceLocator& serviceLocator);
