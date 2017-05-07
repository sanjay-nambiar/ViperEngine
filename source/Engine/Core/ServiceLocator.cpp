#include "Pch.h"
#include "ServiceLocator.h"
#include <stdexcept>

namespace Viper
{

	ServiceLocator::ServiceLocator() : logger(nullptr), memoryAllocator(nullptr), audioManager(nullptr)
	{
	};

	void ServiceLocator::Provide(Logger& loggerRef)
	{
		logger = &loggerRef;
	}

	void ServiceLocator::Provide(MemoryAllocator& memoryAllocatorRef)
	{
		memoryAllocator = &memoryAllocatorRef;
		assert(memoryAllocator != nullptr);
	}

	void ServiceLocator::Provide(AudioManager& audioManagerRef)
	{
		audioManager = &audioManagerRef;
	}

	Logger& ServiceLocator::GetLogger() const
	{
		assert(logger != nullptr);
		return (*logger);
	}

	MemoryAllocator& ServiceLocator::GetMemoryAllocator() const
	{
		assert(memoryAllocator != nullptr);
		return (*memoryAllocator);
	}

	AudioManager& ServiceLocator::GetAudioManager() const
	{
		assert(audioManager != nullptr);
		return (*audioManager);
	}

	void ServiceLocator::ValidateServices() const
	{
		if (!(/*logger != nullptr &&  */memoryAllocator != nullptr && audioManager != nullptr))
		{
			throw std::runtime_error("One or more services not configured.");
		}
	}
}
