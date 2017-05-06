#include "Pch.h"
#include "ServiceLocator.h"

namespace Viper
{

	ServiceLocator::ServiceLocator() : logger(nullptr), memoryAllocator(nullptr), audioManager(nullptr)
	{
	};

	void ServiceLocator::Provide(Logger* loggerPtr)
	{
		this->logger = loggerPtr;
	}

	void ServiceLocator::Provide(MemoryAllocator* memoryAllocatorPtr)
	{
		this->memoryAllocator = memoryAllocatorPtr;
	}

	void ServiceLocator::Provide(AudioManager* audioManagerPtr)
	{
		this->audioManager = audioManagerPtr;
	}

	Logger& ServiceLocator::GetLogger()
	{
		assert(sInstance->logger != nullptr);
		return (*sInstance->logger);
	}

	MemoryAllocator& ServiceLocator::GetMemoryAllocator()
	{
		assert(sInstance->memoryAllocator != nullptr);
		return (*sInstance->memoryAllocator);
	}

	AudioManager& ServiceLocator::GetAudioManager()
	{
		assert(sInstance->audioManager != nullptr);
		return (*sInstance->audioManager);
	}
}
