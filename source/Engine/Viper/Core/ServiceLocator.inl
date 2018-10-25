#include <cassert>
#include <type_traits>
#include "GameException.h"

namespace Viper
{
	template<typename ServiceT>
	ServiceT& ServiceLocator::Get()
	{
		static_assert(std::is_base_of<Service, ServiceT>::value, "ServiceT must inherit from Service");
		assert(services.find(ServiceT::Type) != services.end());
		throw GameException("Not implemented");
	}

	template<>
	Assets::AssetManager& ServiceLocator::Get<Assets::AssetManager>();

	template<>
	Audio::AudioManager& ServiceLocator::Get<Audio::AudioManager>();

	template<>
	Gameplay::Game& ServiceLocator::Get<Gameplay::Game>();

	template<>
	Graphics::Renderer& ServiceLocator::Get<Graphics::Renderer>();

	template<>
	Input::InputManager& ServiceLocator::Get<Input::InputManager>();

	template<>
	Logging::Logger& ServiceLocator::Get<Logging::Logger>();

	template<>
	Memory::MemoryAllocator& ServiceLocator::Get<Memory::MemoryAllocator>();

	template<>
	Window::WindowManager& ServiceLocator::Get<Window::WindowManager>();
}
