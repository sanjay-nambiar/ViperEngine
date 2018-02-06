#pragma once

#include "Singleton.h"


namespace Viper
{
	namespace Logging { class Logger;  }
	namespace Memory { class MemoryAllocator; }
	namespace Audio { class AudioManager; }
	namespace Window { class WindowManager; }
	namespace Input { class InputManager; }
	namespace Graphics
	{ 
		class Renderer;
		class TextureLoader;
	}

	/** This is a singleton class which keeps a handle to all the services
	 *  Services should be provided to this class throught the provide method
	 *  There are accessor methods for getting individual services
	 */
	class ServiceLocator final: public Singleton<ServiceLocator>
	{
		friend Singleton<ServiceLocator>;

		Logging::Logger* logger;
		Memory::MemoryAllocator* memoryAllocator;
		Audio::AudioManager* audioManager;
		Window::WindowManager* windowManager;
		Input::InputManager* inputManager;
		Graphics::Renderer* rendererSystem;
		Graphics::TextureLoader* textureLoader;

		ServiceLocator();
		~ServiceLocator() = default;
		
		ServiceLocator(const ServiceLocator&) = delete;
		ServiceLocator(ServiceLocator&&) = delete;
		ServiceLocator& operator=(const ServiceLocator&) = delete;
		ServiceLocator& operator=(ServiceLocator&&) = delete;
	public:
		/** Provide a logger implementation to the service locator
		 *  @param logger A Logger interface implementation
		 */
		void Provide(Logging::Logger& logger);

		/** Provide a memory allocator implementation to the service locator
		 *  @param memoryAllocator A MemoryAllocator interface implementation
		 */
		void Provide(Memory::MemoryAllocator& memoryAllocator);

		/** Provide an audio manager implementation to the service locator
		 *  @param audioManager An AudioManager interface implementation
		 */
		void Provide(Audio::AudioManager& audioManager);

		/** Provide a window manager implementation to the service locator
		 *  @param windowManager A WindowManager interface implementation
		 */
		void Provide(Window::WindowManager& windowManager);

		/** Provide an input manager implementation to the service locator
		 *  @param inputManager An InputManager interface implementation
		 */
		void Provide(Input::InputManager& inputManager);

		/** Provide a renderer system implementation to the service locator
		 *  @param rendererSystem A RendererSystem interface implementation
		 */
		void Provide(Graphics::Renderer& rendererSystem);

		/** Provide a textureLoader system implementation to the service locator
		 *  @param textureLoader A TextureLoader interface implementation
		 */
		void Provide(Graphics::TextureLoader& textureLoader);

		/** Gets a reference to the default Logger
		 *  @return A referece to the default logger
		 */
		Logging::Logger& GetLogger() const;

		/** Gets a reference to the default memory allocator
		 *  @return A referece to the default memory allocator
		 */
		Memory::MemoryAllocator& GetMemoryAllocator() const;

		/** Gets a reference to the default AudioManager
		 *  @return A referece to the default AudioManager
		 */
		Audio::AudioManager& GetAudioManager() const;

		/** Gets a reference to the default WindowManager
		 *  @return A referece to the default WindowManager
		 */
		Window::WindowManager& GetWindowManager() const;

		/** Gets a reference to the default InputManager
		 *  @return A referece to the default InputManager
		 */
		Input::InputManager& GetInputManager() const;

		/** Gets a reference to the default RendererSystem
		 *  @return A referece to the default RendererSystem
		 */
		Graphics::Renderer& GetRendererSystem() const;

		/** Gets a reference to the default TextureLoader
		 *  @return A referece to the default TextureLoader
		 */
		Graphics::TextureLoader& GetTextureLoader() const;

		/** Checks if all services are loaded. Throws exception if any service is not configured
		 */
		void ValidateServices() const;
	};
}
