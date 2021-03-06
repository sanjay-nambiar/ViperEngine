#include "Viper.h"
#include "Platform/ModuleImports.h"
#include "Platform/ModuleLoader.h"

using namespace std;

namespace Viper
{
	namespace Platform
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
			ServiceLocator::GetInstance().Validate();
		}

		void ModuleLoader::UnloadModules()
		{
			//TODO: investigate this special case for audio manager
			auto& audioManager = ServiceLocator::GetInstance().Get<Audio::AudioManager>();
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
			ifstream file;
			file.open(configFile);
			if (!file.is_open())
			{
				throw GameException("Unable to read module config file: " + configFile);
			}

			string configJson((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

			char *endptr;
			JsonValue jsonObject;
			JsonAllocator jsonAllocator;
			int status = jsonParse(const_cast<char*>(configJson.c_str()), &endptr, &jsonObject, jsonAllocator);
			if (status != JSON_OK)
			{
				throw GameException(string(jsonStrError(status)) + " " + to_string(static_cast<size_t>(endptr - configJson.c_str())));
			}

			for (const auto& sectionIt : jsonObject)
			{
				auto& section = sectionIt->value;
				assert(section.getTag() == JSON_OBJECT);
				string sectionName(sectionIt->key);
				for (const auto& paramsIt : section)
				{
					auto& param = paramsIt->value;
					assert(param.getTag() == JSON_STRING);
					configData[sectionName].insert({ string(paramsIt->key),  string(param.toString())});
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
