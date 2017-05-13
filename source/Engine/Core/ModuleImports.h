#pragma once

#include <string>
#include <unordered_map>
#include "DynamicLibrary.h"


namespace Viper
{
	static const std::string InitializeModuleProcName = "InitializeModule";
	class ServiceLocator;
}

// Import declarations
extern "C" MODULE_IMPORT void InitializeModule(Viper::ServiceLocator& serviceLocator, const std::unordered_map<std::string, std::string>& configAttributes);

// Function pointers
typedef void(__cdecl *funcInitializeModule) (Viper::ServiceLocator&, const std::unordered_map<std::string, std::string>&);
