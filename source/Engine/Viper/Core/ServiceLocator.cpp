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
	}

	void ServiceLocator::Provide(Service& service)
	{
		services.insert({ service.Type(), &service });
	}

	template<>
	AudioManager& ServiceLocator::Get<AudioManager>()
	{
		return *(static_cast<AudioManager*>(services[ServiceType::AudioManager]));
	}

	template<>
	Game& ServiceLocator::Get<Game>()
	{
		return *(static_cast<Game*>(services[ServiceType::Game]));
	}

	template<>
	Renderer& ServiceLocator::Get<Renderer>()
	{
		return *(static_cast<Renderer*>(services[ServiceType::Renderer]));
	}

	template<>
	TextureLoader& ServiceLocator::Get<TextureLoader>()
	{
		return *(static_cast<TextureLoader*>(services[ServiceType::TextureLoader]));
	}

	template<>
	InputManager& ServiceLocator::Get<InputManager>()
	{
		return *(static_cast<InputManager*>(services[ServiceType::InputManager]));
	}

	template<>
	Logger& ServiceLocator::Get<Logger>()
	{
		return *(static_cast<Logger*>(services[ServiceType::Logger]));
	}

	template<>
	MemoryAllocator& ServiceLocator::Get<MemoryAllocator>()
	{
		return *(static_cast<MemoryAllocator*>(services[ServiceType::MemoryAllocator]));
	}

	template<>
	WindowManager& ServiceLocator::Get<WindowManager>()
	{
		return *(static_cast<WindowManager*>(services[ServiceType::WindowManager]));
	}

	void ServiceLocator::Validate() const
	{
		//TODO: fill in
	}
}
