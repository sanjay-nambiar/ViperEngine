#pragma once

#include <string>
#include <unordered_map>
#include "DynamicLibrary.h"

namespace Viper
{
	class ServiceLocator;
}

// Export declarations
extern "C" MODULE_EXPORT void InitializeModule(Viper::ServiceLocator& serviceLocator, const std::unordered_map<std::string, std::string>& configAttributes);
