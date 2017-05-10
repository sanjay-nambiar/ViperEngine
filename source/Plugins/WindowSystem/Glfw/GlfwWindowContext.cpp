#include "GlfwWindowContext.h"

namespace Viper
{
	namespace Window
	{
		bool GlfwWindowContext::operator==(const GlfwWindowContext& rhs) const
		{
			return (window == rhs.window);
		}

		bool GlfwWindowContext::operator!=(const GlfwWindowContext& rhs) const
		{
			return !operator==(rhs);
		}
	}
}
