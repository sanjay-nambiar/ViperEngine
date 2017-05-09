#include "Pch.h"
#include "ServiceLocator.h"
#include <stdexcept>

namespace Viper
{

	ServiceLocator::ServiceLocator() :
		logger(nullptr), memoryAllocator(nullptr), audioManager(nullptr), windowManager(nullptr)
	{
	};

	void ServiceLocator::Provide(Logger& loggerRef)
	{
		logger = &loggerRef;
	}

	void ServiceLocator::Provide(MemoryAllocator& memoryAllocatorRef)
	{
		memoryAllocator = &memoryAllocatorRef;
	}

	void ServiceLocator::Provide(AudioManager& audioManagerRef)
	{
		audioManager = &audioManagerRef;
	}

	void ServiceLocator::Provide(WindowManager& windowManagerRef)
	{
		windowManager = &windowManagerRef;
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

	WindowManager& ServiceLocator::GetWindowManager() const
	{
		assert(windowManager != nullptr);
		return (*windowManager);
	}

	void ServiceLocator::ValidateServices() const
	{
		if (!(/*logger != nullptr &&  */memoryAllocator != nullptr && audioManager != nullptr))
		{
			throw std::runtime_error("One or more services not configured.");
		}
	}
}
