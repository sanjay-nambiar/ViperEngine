#pragma once

#include <unordered_map>
#include "Service.h"
#include "Singleton.h"

namespace Viper
{
	// Forward declarations
	class Service;
	namespace Audio { class AudioManager; }
	namespace Assets { class AssetManager; }
	namespace Gameplay { class Game; }
	namespace Graphics
	{
		class Renderer;
		class TextureLoader;
	}
	namespace Input { class InputManager; }
	namespace Logging { class Logger;  }
	namespace Memory { class MemoryAllocator; }
	namespace Window { class WindowManager; }

	/** This is a singleton class which keeps a handle to all the services
	 *  Services should be provided to this class throught the provide method
	 *  There are accessor methods for getting individual services
	 */
	class ServiceLocator final: public Singleton<ServiceLocator>
	{
	public:
		/** Provide an concrete service implementation to the service locator
		*  @param service A concrete service interface implementation
		*/
		void Provide(Service& service);

		/** Gets a reference to the requested service
		*  @return A referece to the requested service
		*/
		template<typename ServiceT>
		ServiceT& Get();

		/** Checks if all services are loaded. Throws exception if any service is not configured
		 */
		void Validate() const;

	private:
		ServiceLocator();
		~ServiceLocator() = default;

		ServiceLocator(const ServiceLocator&) = delete;
		ServiceLocator(ServiceLocator&&) = delete;
		ServiceLocator& operator=(const ServiceLocator&) = delete;
		ServiceLocator& operator=(ServiceLocator&&) = delete;

		friend Singleton<ServiceLocator>;

		std::unordered_map<ServiceType, Service*> services;
		Assets::AssetManager* assetManager;
		Audio::AudioManager* audioManager;
		Gameplay::Game* game;
		Graphics::Renderer* rendererSystem;
		Graphics::TextureLoader* textureLoader;
		Input::InputManager* inputManager;
		Logging::Logger* logger;
		Memory::MemoryAllocator* memoryAllocator;
		Window::WindowManager* windowManager;
	};
}

#include "ServiceLocator.inl"
