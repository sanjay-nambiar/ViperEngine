#include "ServiceLocator.h"

namespace Viper
{

	ServiceLocator::ServiceLocator() : logger(nullptr), memoryAllocator(nullptr), audioManager(nullptr)
	{
	};

	void ServiceLocator::Provide(Logger* logger)
	{
		this->logger = logger;
	}

	void ServiceLocator::Provide(MemoryAllocator* memoryAllocator)
	{
		this->memoryAllocator = memoryAllocator;
	}

	void ServiceLocator::Provide(AudioManager* audioManager)
	{
		this->audioManager = audioManager;
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
