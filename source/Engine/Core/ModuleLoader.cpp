#include "Pch.h"
#include "ModuleLoader.h"
#include <fstream>
#include "Core/ModuleImports.h"
#include "Service/AudioManager.h"
#include <regex>


namespace Viper
{
	namespace Core
	{
		const std::regex ModuleLoader::SectionTagPattern = std::regex("^\\s*\\[\\s*([A-Za-z0-9_]+)\\s*\\]\\s*$");
		const std::regex ModuleLoader::AttributeLinePattern = std::regex("^\\s*([^=]+)=(.*)$");

		ModuleLoader::~ModuleLoader()
		{
			UnloadModules();
		}

		void ModuleLoader::LoadModules(const std::string& configFilePath)
		{
			ServiceLocator::CreateInstance(*allocator);
			ServiceLocator::GetInstance().Provide(*allocator);

			configFile = configFilePath;
			LoadConfigData();
			LoadModulesFromConfig();
			ServiceLocator::GetInstance().ValidateServices();
		}

		void ModuleLoader::UnloadModules()
		{
			AudioManager& audioManager = ServiceLocator::GetInstance().GetAudioManager();
			ServiceLocator::Destroy();
			delete &audioManager;

			for (auto& moduleHandle : moduleHandles)
			{
				DYNAMIC_LIB_UNLOAD(moduleHandle.handle);
			}
			moduleHandles.clear();
			configData.clear();
			configFile.clear();
		}

		void ModuleLoader::LoadConfigData()
		{
			std::ifstream file;
			file.open(configFile);
			if (!file.is_open())
			{
				throw std::runtime_error("Unable to read module config file: " + configFile);
			}

			std::string line;
			std::string sectionName;
			while (std::getline(file, line))
			{
				std::smatch matches;
				if (std::regex_search(line, matches, SectionTagPattern))
				{
					sectionName = matches.str(1);
				}
				else if (std::regex_search(line, matches, AttributeLinePattern))
				{
					configData[sectionName].insert({matches.str(1), matches.str(2)});
				}
			}
			file.close();
		}

		void ModuleLoader::LoadModulesFromConfig()
		{
			for (const auto& entry : configData)
			{
				const auto& sectionName = entry.first;
				const auto& attributes = entry.second;
				assert(attributes.find("Plugin") != attributes.end());
				const auto& moduleName = attributes.at("Plugin");
				DYNAMIC_LIB_HANDLE libInstance = DYNAMIC_LIB_LOAD(moduleName);
				if (libInstance == nullptr)
				{
					throw "Unable to load " + moduleName + "from section " + sectionName;
				}
				ModuleHandle handle = {libInstance};
				moduleHandles.emplace_back(handle);

				funcInitializeModule initializeModule = reinterpret_cast<funcInitializeModule>(DYNAMIC_LIB_GETSYM(libInstance, InitializeModuleProcName.c_str()));
				assert(initializeModule != nullptr);
				initializeModule(ServiceLocator::GetInstance(), attributes);
			}
		}
	}
}
