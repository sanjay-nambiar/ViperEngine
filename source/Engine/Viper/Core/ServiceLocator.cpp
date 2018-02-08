#include "Viper.h"
#include "ServiceLocator.h"
#include "Logging/Logger.h"
#include "Memory/MemoryAllocator.h"
#include "Audio/AudioManager.h"
#include "Window/WindowManager.h"
#include "Input/InputManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/TextureLoader.h"
#include <stdexcept>

namespace Viper
{
	using namespace Logging;
	using namespace Memory;
	using namespace Audio;
	using namespace Window;
	using namespace Input;
	using namespace Graphics;

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

	void ServiceLocator::Provide(InputManager& inputManagerRef)
	{
		inputManager = &inputManagerRef;
	}

	void ServiceLocator::Provide(Renderer& rendererSystemRef)
	{
		rendererSystem = &rendererSystemRef;
	}

	void ServiceLocator::Provide(TextureLoader& textureLoaderRef)
	{
		textureLoader = &textureLoaderRef;
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

	InputManager& ServiceLocator::GetInputManager() const
	{
		assert(inputManager != nullptr);
		return (*inputManager);
	}

	Renderer& ServiceLocator::GetRendererSystem() const
	{
		assert(rendererSystem != nullptr);
		return (*rendererSystem);
	}

	TextureLoader& ServiceLocator::GetTextureLoader() const
	{
		assert(textureLoader != nullptr);
		return (*textureLoader);
	}

	void ServiceLocator::ValidateServices() const
	{
		if (!(/*logger != nullptr &&  */memoryAllocator != nullptr && audioManager != nullptr))
		{
			throw std::runtime_error("One or more services not configured.");
		}
	}
}
