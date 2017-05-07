#pragma once

#include <fstream>
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

			void MoveToSection(const std::string& sectionName);
			void GetAttributes(const std::string& sectionName, std::unordered_map<std::string, std::string>& attributesMap);
			void InitializeAudio();
			void InitializeRenderer();

			template <typename ProvideModuleMethodT>
			static ProvideModuleMethodT LoadModuleFromDll(const std::string& moduleName, const std::string& sectionName);

			std::ifstream configFile;

			static const std::string AudioModuleSection;
			static const std::string RendererModuleSection;
		};
	}
}
