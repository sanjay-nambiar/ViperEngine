#include "Viper.h"
#include "Platform/ModuleImports.h"
#include "Platform/ModuleLoader.h"

using namespace std;

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
			Audio::AudioManager& audioManager = ServiceLocator::GetInstance().GetAudioManager();
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
				throw runtime_error("Unable to read module config file: " + configFile);
			}

			string configJson((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

			char *endptr;
			JsonValue jsonObject;
			JsonAllocator jsonAllocator;
			int status = jsonParse(const_cast<char*>(configJson.c_str()), &endptr, &jsonObject, jsonAllocator);
			if (status != JSON_OK)
			{
				throw runtime_error(string(jsonStrError(status)) + " " + to_string(static_cast<size_t>(endptr - configJson.c_str())));
			}

			for (auto sectionIt = begin(jsonObject); sectionIt != JsonIterator::end(); ++sectionIt)
			{
				auto& section = sectionIt->value;
				assert(section.getTag() == JSON_OBJECT);
				string sectionName(sectionIt->key);
				for (auto paramsIt = begin(section); paramsIt != JsonIterator::end(); ++paramsIt)
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
