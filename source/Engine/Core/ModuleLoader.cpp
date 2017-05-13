#include "Pch.h"
#include "ModuleLoader.h"
#include <fstream>
#include "Core/ModuleImports.h"
#include "Service/AudioManager.h"


namespace Viper
{
	namespace Core
	{
		const std::string ModuleLoader::AudioModuleSection = "Audio";
		const std::string ModuleLoader::RendererModuleSection = "Renderer";
		const std::string ModuleLoader::WindowSystemModuleSection = "WindowSystem";

		void ModuleLoader::LoadModules(const std::string& configFilePath)
		{
			ServiceLocator::CreateInstance(*allocator);
			ServiceLocator::GetInstance().Provide(*allocator);

			configFile = configFilePath;
			LoadConfigData();
			InitializeWindowSystem();
			InitializeAudio();
			InitializeRenderer();
			ServiceLocator::GetInstance().ValidateServices();
		}

		void ModuleLoader::UnloadModules()
		{
			AudioManager& audioManager = ServiceLocator::GetInstance().GetAudioManager();
			ServiceLocator::Destroy();
			delete &audioManager;
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

		void ModuleLoader::InitializeWindowSystem()
		{
			assert(configData.find(WindowSystemModuleSection) != configData.end());
			std::unordered_map<std::string, std::string>& attributes = configData[WindowSystemModuleSection];
			auto window = LoadModuleFromDll<funcProvideWindowSystem>(attributes["Plugin"], "WindowSystem");
			assert(window != nullptr);
			window(ServiceLocator::GetInstance());
		}

		void ModuleLoader::InitializeAudio()
		{
			assert(configData.find(AudioModuleSection) != configData.end());
			std::unordered_map<std::string, std::string>& attributes = configData[AudioModuleSection];
			auto audio = LoadModuleFromDll<funcProvideAudio>(attributes["Plugin"], "Audio");
			assert(audio != nullptr);
			audio(std::atoi(attributes["Channels"].c_str()), ServiceLocator::GetInstance());
		}

		void ModuleLoader::InitializeRenderer()
		{
			assert(configData.find(RendererModuleSection) != configData.end());
			std::unordered_map<std::string, std::string>& attributes = configData[RendererModuleSection];
			auto renderer = LoadModuleFromDll<funcProvideRenderer>(attributes["Plugin"], "Renderer");
			assert(renderer != nullptr);
			renderer(ServiceLocator::GetInstance());
		}

		template <typename ProvideModuleMethodT>
		ProvideModuleMethodT ModuleLoader::LoadModuleFromDll(const std::string& moduleName, const std::string& sectionName)
		{
			DYNAMIC_LIB_HANDLE libInstance = DYNAMIC_LIB_LOAD(moduleName);
			if (libInstance == nullptr)
			{
				throw "Unable to load " + moduleName;
			}

			const std::string methodName = "Provide" + sectionName;
			ProvideModuleMethodT provideAudioManager = reinterpret_cast<ProvideModuleMethodT>(DYNAMIC_LIB_GETSYM(libInstance, methodName.c_str()));
			assert(provideAudioManager != nullptr);
			if (provideAudioManager != nullptr)
			{
				return provideAudioManager;
			}
			bool free = DYNAMIC_LIB_UNLOAD(libInstance);
			throw "Unable to get proc address InitializeAudio. Library unloaded = %d." + free;
		}
	}
}
