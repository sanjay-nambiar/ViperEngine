#include "Pch.h"

using namespace std;
using namespace Viper;
using namespace AssetPipeline;


int PrintUsage(const string& executableName)
{
	cout << "Usage: " << executableName << " <assets source directory> <game content directory>" << endl;
	return 0;
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		return PrintUsage(argv[0]);
	}

	try
	{
		AssetProcessor processor;
		processor.LoadAssetList(argv[1], argv[2]);
		processor.SaveAssets();
	}
	catch (GameException ex)
	{
		cout << "Error: " << ex.what() << endl;
		return 1;
	}
	return 0;
}
