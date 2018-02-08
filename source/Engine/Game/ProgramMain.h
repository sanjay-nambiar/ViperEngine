#pragma once

#include <string>

namespace Viper
{
	namespace Window { struct WindowContext; }

	namespace Platform
	{
		class ProgramMain
		{
		public:
			void Initialize();
			void Update();
			void Shutdown();
		private:
			const Window::WindowContext* windowContext;
			static const std::string ConfigFile;
		};
	}
}
