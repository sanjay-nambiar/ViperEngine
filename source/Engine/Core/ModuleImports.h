#pragma once

#include "Core/MemoryManager.h"
#include "Service/AudioManager.h"


#define DllImport	__declspec(dllimport)


// Import declarations
extern "C" DllImport Viper::AudioManager* InitializeAudio(uint32_t maxChannels, Viper::MemoryManager& memoryManager);

// Function pointers
typedef Viper::AudioManager*(__stdcall *funcInitializeAudio) (uint32_t, Viper::MemoryManager&);
