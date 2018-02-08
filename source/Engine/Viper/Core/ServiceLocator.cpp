#include "Viper.h"

namespace Viper
{
	using namespace Audio;
	using namespace Gameplay;
	using namespace Graphics;
	using namespace Input;
	using namespace Logging;
	using namespace Memory;
	using namespace Window;

	ServiceLocator::ServiceLocator() :
		logger(nullptr), memoryAllocator(nullptr), audioManager(nullptr), windowManager(nullptr)
	{
	};

	void ServiceLocator::Provide(AudioManager& audioManagerRef)
	{
		audioManager = &audioManagerRef;
	}

	void ServiceLocator::Provide(Game& gameRef)
	{
		game = &gameRef;
	}

	void ServiceLocator::Provide(Renderer& rendererSystemRef)
	{
		rendererSystem = &rendererSystemRef;
	}

	void ServiceLocator::Provide(TextureLoader& textureLoaderRef)
	{
		textureLoader = &textureLoaderRef;
	}

	void ServiceLocator::Provide(InputManager& inputManagerRef)
	{
		inputManager = &inputManagerRef;
	}

	void ServiceLocator::Provide(Logger& loggerRef)
	{
		logger = &loggerRef;
	}

	void ServiceLocator::Provide(MemoryAllocator& memoryAllocatorRef)
	{
		memoryAllocator = &memoryAllocatorRef;
	}

	void ServiceLocator::Provide(WindowManager& windowManagerRef)
	{
		windowManager = &windowManagerRef;
	}

	AudioManager& ServiceLocator::GetAudioManager() const
	{
		assert(audioManager != nullptr);
		return (*audioManager);
	}

	Game& ServiceLocator::GetGame() const
	{
		assert(game != nullptr);
		return (*game);
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

	InputManager& ServiceLocator::GetInputManager() const
	{
		assert(inputManager != nullptr);
		return (*inputManager);
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
