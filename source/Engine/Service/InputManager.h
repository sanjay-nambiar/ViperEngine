#pragma once

namespace Viper
{
	class InputManager
	{
	public:
		InputManager() = default;
		virtual ~InputManager() = default;

		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Shutdown() = 0;
	};
}
