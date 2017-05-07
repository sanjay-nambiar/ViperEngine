#include "Pch.h"
#include "ModuleLoader.h"
#include <iostream>
#include <windows.h>
#include "Core/ModuleImports.h"
#include "Service/AudioManager.h"


namespace Viper
{
	namespace Core
	{
		const std::string ModuleLoader::AudioModuleSection = "Audio";
		const std::string ModuleLoader::RendererModuleSection = "Renderer";

		void ModuleLoader::LoadModules(const std::string& configFilePath)
		{
			configFile.open(configFilePath);
			if (!configFile.is_open())
			{
				throw std::runtime_error("Unable to read module config file: " + configFilePath);
			}

			ServiceLocator::CreateInstance(*allocator);
			ServiceLocator::GetInstance().Provide(*allocator);
			InitializeAudio();
			InitializeRenderer();
			ServiceLocator::GetInstance().ValidateServices();
		}

		void ModuleLoader::UnloadModules()
		{
			AudioManager& audioManager = ServiceLocator::GetInstance().GetAudioManager();
			ServiceLocator::Destroy();
			delete &audioManager;
			configFile.close();
		}

		void ModuleLoader::MoveToSection(const std::string& sectionName)
		{
			configFile.seekg(0, std::ios::beg);
			std::string line;
			bool sectionFound = false;
			const std::string sectionHeader = "[" + sectionName + "]";
			while (std::getline(configFile, line))
			{
				if (line == sectionHeader)
				{
					sectionFound = true;
					break;
				}
			}
			
			if (!sectionFound)
			{
				throw std::runtime_error("Unable to find section " + sectionName + " in modules config");
			}
		}

		//TODO: Improve parsing. consider white spaces
		void ModuleLoader::GetAttributes(const std::string& sectionName, std::unordered_map<std::string, std::string>& attributesMap)
		{
			MoveToSection(sectionName);
			std::string line;
			while (std::getline(configFile, line))
			{
				if (line[0] == '[')
				{
					break;
				}

				size_t pos = line.find('=');
				assert(pos < line.length());
				attributesMap.insert({
					line.substr(0, pos),
					line.substr(pos + 1)
				});
			}
		}

		void ModuleLoader::InitializeAudio()
		{
			std::unordered_map<std::string, std::string> attributes;
			GetAttributes(AudioModuleSection, attributes);
			auto audio = LoadModuleFromDll<funcProvideAudio>(attributes["Plugin"], "Audio");
			assert(audio != nullptr);
			audio(std::atoi(attributes["Channels"].c_str()), ServiceLocator::GetInstance());
		}

		void ModuleLoader::InitializeRenderer()
		{
			std::unordered_map<std::string, std::string> attributes;
			GetAttributes(RendererModuleSection, attributes);
		}

		template <typename ProvideModuleMethodT>
		ProvideModuleMethodT ModuleLoader::LoadModuleFromDll(const std::string& moduleName, const std::string& sectionName)
		{
			const std::string dllName = moduleName + ".dll";
			std::wstring wideStr(dllName.begin(), dllName.end());
			HINSTANCE dllInstance = LoadLibrary(wideStr.c_str());
			if (dllInstance == nullptr)
			{
				throw "Unable to load " + dllName;
			}

			const std::string methodName = "Provide" + sectionName;
			ProvideModuleMethodT provideAudioManager = reinterpret_cast<ProvideModuleMethodT>(GetProcAddress(dllInstance, methodName.c_str()));
			assert(provideAudioManager != nullptr);
			if (provideAudioManager != nullptr)
			{
				return provideAudioManager;
			}
			FreeLibrary(dllInstance);
			throw "Unable to get proc address InitializeAudio";
		}
	}
}
