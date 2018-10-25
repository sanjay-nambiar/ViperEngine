#include "GlfwWindowContext.h"

using namespace std;

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

		uint64_t GlfwWindowContext::WindowHandle() const
		{
			return reinterpret_cast<uint64_t>(window);
		}
	}
}
