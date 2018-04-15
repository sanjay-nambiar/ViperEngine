#include "Viper.h"

using namespace std;

namespace Viper
{
	string Utility::GetFilenameWithoutExtension(const string& fullFilename)
	{
		size_t index = fullFilename.find_last_of(".");
		if (index == string::npos)
		{
			index = fullFilename.size();
		}
		return fullFilename.substr(0, index);
	}
}
