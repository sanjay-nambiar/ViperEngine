#pragma once

#include "Singleton.h"
#include "Service/AudioManager.h"
#include "Service/InputManager.h"
#include "Service/MemoryAllocator.h"
#include "Service/Logger.h"
#include "Service/RendererSystem.h"
#include "Service/WindowManager.h"


namespace Viper
{
	/** This is a singleton class which keeps a handle to all the services
	 *  Services should be provided to this class throught the provide method
	 *  There are accessor methods for getting individual services
	 */
	class ServiceLocator final: public Singleton<ServiceLocator>
	{
		friend Singleton<ServiceLocator>;

		Logger* logger;
		MemoryAllocator* memoryAllocator;
		AudioManager* audioManager;
		WindowManager* windowManager;
		InputManager* inputManager;
		RendererSystem* rendererSystem;

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
		void Provide(Logger& logger);

		/** Provide a memory allocator implementation to the service locator
		 *  @param memoryAllocator A MemoryAllocator interface implementation
		 */
		void Provide(MemoryAllocator& memoryAllocator);

		/** Provide an audio manager implementation to the service locator
		 *  @param audioManager An AudioManager interface implementation
		 */
		void Provide(AudioManager& audioManager);

		/** Provide a window manager implementation to the service locator
		 *  @param windowManager A WindowManager interface implementation
		 */
		void Provide(WindowManager& windowManager);

		/** Provide an input manager implementation to the service locator
		 *  @param inputManager An InputManager interface implementation
		 */
		void Provide(InputManager& inputManager);

		/** Provide a renderer system implementation to the service locator
		 *  @param rendererSystem A RendererSystem interface implementation
		 */
		void Provide(RendererSystem& rendererSystem);

		/** Gets a reference to the default Logger
		 *  @return A referece to the default logger
		 */
		Logger& GetLogger() const;

		/** Gets a reference to the default memory allocator
		 *  @return A referece to the default memory allocator
		 */
		MemoryAllocator& GetMemoryAllocator() const;

		/** Gets a reference to the default AudioManager
		 *  @return A referece to the default AudioManager
		 */
		AudioManager& GetAudioManager() const;

		/** Gets a reference to the default WindowManager
		 *  @return A referece to the default WindowManager
		 */
		WindowManager& GetWindowManager() const;

		/** Gets a reference to the default InputManager
		 *  @return A referece to the default InputManager
		 */
		InputManager& GetInputManager() const;

		/** Gets a reference to the default RendererSystem
		 *  @return A referece to the default RendererSystem
		 */
		RendererSystem& GetRendererSystem() const;

		/** Checks if all services are loaded. Throws exception if any service is not configured
		 */
		void ValidateServices() const;
	};
}
