#pragma once

#include <string>

namespace Viper
{
	namespace Logging
	{
		/** This is an interface for logger implementations
		*/
		class Logger
		{
		public:
			/** Writes debug level logs to the log stream
			 * @param message The debug log message
			*/
			virtual void Debug(std::string& message) = 0;
			/** Writes error level logs to the log stream
			 * @param message The error log message
			*/
			virtual void Error(std::string& message) = 0;
			/** Writes warning level logs to the log stream
			 * @param message The warn log message
			*/
			virtual void Warn(std::string& message) = 0;

			virtual ~Logger() = 0;
		};
	}
}
