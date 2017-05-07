#pragma once

#include "Core/MemoryManager.h"
#include "Service/AudioManager.h"


#define DllExport	__declspec(dllexport)


// All module initialization export declarations
extern "C" DllExport Viper::AudioManager* InitializeAudio(uint32_t maxChannels, Viper::MemoryManager& memoryManager);
