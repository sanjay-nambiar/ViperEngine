#include "Pch.h"
#include "ServiceLocator.h"

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
}
