#include "Pch.h"

using namespace std;
using namespace Viper;
using namespace AssetPipeline;


int PrintUsage(const string& executableName)
{
	cout << "Usage: " << executableName << " <assets source directory> <game content directory> [-debug]" << endl;
	return 0;
}

int main(int argc, char* argv[])
{
	if (argc < 3 || argc > 4)
	{
		return PrintUsage(argv[0]);
	}

	try
	{
		string resourceDir(argv[1]);
		string contentDir(argv[2]);
		bool isDebug = (argc == 4) ? (string(argv[3]) == "-debug") : false;

		cout << "Starting " << argv[0] << " tool..." << endl;
		cout << "Resources directory : " << resourceDir << endl;
		cout << "Content directory : " << contentDir << endl;
		cout << "Is debug mode on? : " << boolalpha << isDebug << endl;

		AssetProcessor processor;
		processor.LoadAssetList(resourceDir, contentDir);
		processor.SaveAssets(isDebug);
		if (!isDebug)
		{
			processor.Cleanup(contentDir);
		}
		cout << "Tool run complete." << endl;
	}
	catch (GameException ex)
	{
		cout << "Error: " << ex.what() << endl;
		return 1;
	}
	return 0;
}
