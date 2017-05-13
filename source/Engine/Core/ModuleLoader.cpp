#include "Pch.h"
#include "ModuleLoader.h"
#include <fstream>
#include "Core/ModuleImports.h"
#include "Service/AudioManager.h"


namespace Viper
{
	namespace Core
	{
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

		//TODO: Improve parsing. consider white spaces
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
				if (line.size() > 0 && line[0] == '[' && line[line.size() - 1] == ']')
				{
					sectionName = line.substr(1, line.length() - 2);
				}
				else
				{
					size_t pos = line.find('=');
					assert(pos < line.length());
					configData[sectionName].insert({
						line.substr(0, pos),
						line.substr(pos + 1)
					});
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
