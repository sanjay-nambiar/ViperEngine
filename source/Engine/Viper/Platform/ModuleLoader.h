#pragma once

#include <regex>
#include <unordered_map>
#include "Core/Singleton.h"
#include "DynamicLibrary.h"

namespace Viper
{
	namespace Core
	{
		class ModuleLoader final: public Singleton<ModuleLoader>
		{
		public:
			void LoadModules(const std::string& configFilePath);
			void UnloadModules();
		private:
			struct ModuleHandle
			{
				DYNAMIC_LIB_HANDLE handle;
			};

			friend Singleton<ModuleLoader>;

			ModuleLoader() = default;
			~ModuleLoader();

			ModuleLoader(const ModuleLoader&) = delete;
			ModuleLoader(ModuleLoader&&) = delete;
			ModuleLoader& operator=(const ModuleLoader&) = delete;
			ModuleLoader& operator=(ModuleLoader&&) = delete;

			void LoadConfigData();
			void LoadModulesFromConfig();

			std::string configFile;
			std::unordered_map<std::string, std::unordered_map<std::string, std::string>> configData;
			std::vector<ModuleHandle> moduleHandles;
		};
	}
}
