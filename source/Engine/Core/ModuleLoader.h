#pragma once

#include <unordered_map>

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
			friend Singleton<ModuleLoader>;

			ModuleLoader() = default;
			~ModuleLoader() = default;

			ModuleLoader(const ModuleLoader&) = delete;
			ModuleLoader(ModuleLoader&&) = delete;
			ModuleLoader& operator=(const ModuleLoader&) = delete;
			ModuleLoader& operator=(ModuleLoader&&) = delete;

			void LoadConfigData();

			void InitializeAudio();
			void InitializeRenderer();
			void InitializeWindowSystem();

			template <typename ProvideModuleMethodT>
			static ProvideModuleMethodT LoadModuleFromDll(const std::string& moduleName, const std::string& sectionName);

			std::string configFile;
			std::unordered_map<std::string, std::unordered_map<std::string, std::string>> configData;

			static const std::string AudioModuleSection;
			static const std::string RendererModuleSection;
			static const std::string WindowSystemModuleSection;
		};
	}
}
