#pragma once

namespace Viper
{
	enum class ServiceType
	{
		AudioManager,
		AssetManager,
		Game,
		InputManager,
		Logger,
		MemoryAllocator,
		Renderer,
		TextureLoader,
		WindowManager
	};

	class Service
	{
	public:
		Service(ServiceType type);
		ServiceType Type() const;
	protected:
		ServiceType type;
	};
}
