#pragma once

#include "Singleton.h"

namespace Viper
{
	class Logger;
	class MemoryAllocator;
	class AudioManager;

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

		/** Checks if all services are loaded. Throws exception if any service is not configured
		 */
		void ValidateServices() const;
	};
}
