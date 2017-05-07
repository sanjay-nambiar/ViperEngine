#include "Pch.h"
#include <windows.h>
#include <strsafe.h>
#include "Core/MemoryManager.h"
#include "Core/ModuleImports.h"
#include "Core/ServiceLocator.h"
#include "Service/AudioManager.h"

#ifndef UNICODE
#define UNICODE
#endif


using namespace Viper;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

funcProvideAudioManager LoadModule()
{
	HINSTANCE hDLL = LoadLibrary(L"FmodAudio.dll");
	if (hDLL == nullptr)
	{
		throw "Unable to load FmodAudio.dll";
	}
	else
	{
		funcProvideAudioManager provideAudioManager = reinterpret_cast<funcProvideAudioManager>(GetProcAddress(hDLL, "ProvideAudioManager"));
		assert(provideAudioManager != nullptr);
		if (provideAudioManager != nullptr)
		{
			return provideAudioManager;
		}
		FreeLibrary(hDLL);
		throw "Unable to get proc address InitializeAudio";
	}
}

void Initialize()
{
	MemoryManager* allocator = new MemoryManager();
	assert(allocator != nullptr);
	ServiceLocator::CreateInstance(*allocator);
	ServiceLocator::GetInstance().Provide(*allocator);
}

void ShutDown()
{
	AudioManager& audioManager = ServiceLocator::GetInstance().GetAudioManager();
	MemoryAllocator& memoryAllocator = ServiceLocator::GetInstance().GetMemoryAllocator();
	ServiceLocator::Destroy();
	delete &audioManager;
	delete &memoryAllocator;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(pCmdLine);

	// Register the window class.
	const wchar_t CLASS_NAME[] = L"ViperEngine Window";

	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	RegisterClass(&wc);

	// Create the window.
	HWND hwnd = CreateWindowEx(
		0,             // Optional window styles.
		CLASS_NAME,
		L"Bobsled Battle",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, // Position
		1280, 720,     // Size
		nullptr,       // Parent windowvi ~
		nullptr,       // Menu
		hInstance,     // Instance handle
		nullptr        // Additional application data
	);
	if (hwnd == nullptr)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	

	Initialize();

	// Load audio manager
	funcProvideAudioManager audio = LoadModule();
	assert(audio != nullptr);
	audio(100, ServiceLocator::GetInstance());

	AudioManager& manager = ServiceLocator::GetInstance().GetAudioManager();
	manager.SetListener3dAttributes(Viper::Vector3(0, 0, 0), Viper::Vector3(0, 0, 1), Viper::Vector3(0, 1, 0));
	manager.LoadSoundBank("sounds/ZombieWars.bank");
	manager.LoadSoundBank("sounds/ZombieWars.strings.bank");
	manager.LoadSoundBankEvents("sounds/ZombieWars.bank");

	Viper::Vector3 position(0, 0, 0);
	manager.SetEvent3dAttributes("event:/GattlingGun-Fire", position, Viper::Vector3(0, 0, 0));
	manager.PlayEvent("event:/GattlingGun-Fire");





	// Run the message loop.
	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		manager.Update();
		position.z += 0.5;
		manager.SetEvent3dAttributes("event:/GattlingGun-Fire", position, Viper::Vector3(0, 0, 0));
		Sleep(500);
	}
	ShutDown();
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
	}
	return 0;

	default:
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
