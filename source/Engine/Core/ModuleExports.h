#pragma once

#include "Service/AudioManager.h"
#include "DynamicLibrary.h"

namespace Viper
{
	class ServiceLocator;
}

// All module initialization export declarations
extern "C" MODULE_EXPORT void ProvideAudio(uint32_t maxChannels, Viper::ServiceLocator& serviceLocator);
extern "C" MODULE_EXPORT void ProvideWindowSystem(Viper::ServiceLocator& serviceLocator);
extern "C" MODULE_EXPORT void ProvideRenderer(Viper::ServiceLocator& serviceLocator);
