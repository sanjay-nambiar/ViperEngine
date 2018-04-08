#include "Pch.h"

using namespace std;
using namespace Viper;
using namespace AssetPipeline;


struct ProgramArgs
{
	string resourceDir;
	string contentDir;
	bool isDebug;
	bool isPackaged;
	bool isParseSuccess;
};

int PrintUsage(const string& executableName);
ProgramArgs ParseArgs(uint32_t argc, char* argv[]);

int main(int argc, char* argv[])
{
	ProgramArgs args = ParseArgs(static_cast<uint32_t>(argc), argv);
	if (!args.isParseSuccess)
	{
		PrintUsage(argv[0]);
		return 1;
	}

	try
	{
		cout << "Starting " << argv[0] << " tool..." << endl;
		cout << "Resources directory : " << args.resourceDir << endl;
		cout << "Content directory : " << args.contentDir << endl;
		cout << "Is debug mode on? : " << boolalpha << args.isDebug << endl;
		cout << "Is package mode on? : " << boolalpha << args.isPackaged << endl;

		AssetProcessor processor;
		processor.GenerateAssetList(args.resourceDir, args.contentDir, args.isPackaged);
		processor.LoadAssetList();
		processor.SaveAssets(args.isDebug);
		if (!args.isDebug)
		{
			processor.Cleanup(args.contentDir);
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

int PrintUsage(const string& executableName)
{
	cout << "Usage: " << executableName << " <assets source directory> <game content directory> [-debug] [-packaged]" << endl;
	return 0;
}

ProgramArgs ParseArgs(uint32_t argc, char* argv[])
{
	ProgramArgs args = {"", "", false, false, false};
	if (argc < 3 || argc > 5)
	{
		return args;
	}

	args.resourceDir = argv[1];
	args.contentDir = argv[2];
	for (uint32_t index = 3; index < argc; ++index)
	{
		string arg = argv[index];
		if (arg == "-debug")
		{
			args.isDebug = true;
		}
		else if (arg == "-packaged")
		{
			args.isPackaged = true;
		}
	}

	args.isParseSuccess = true;
	return args;
}
