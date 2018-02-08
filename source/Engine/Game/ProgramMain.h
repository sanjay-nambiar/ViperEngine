#pragma once

#include <string>

namespace Viper
{
	struct WindowContext;

	namespace Platform
	{
		class ProgramMain
		{
		public:
			void Initialize();
			void Update();
			void Shutdown();
		private:
			const WindowContext* windowContext;
			static const std::string ConfigFile;
		};
	}
}
