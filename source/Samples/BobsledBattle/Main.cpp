#include "Pch.h"
#include <windows.h>
#include <strsafe.h>
#include "Core/MemoryManager.h"
#include "Core/ModuleImports.h"
#include "Service/AudioManager.h"


#ifndef UNICODE
#define UNICODE
#endif


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void ErrorExit(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, nullptr);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR), TEXT("%s failed with error %d: %s"), lpszFunction, dw, lpMsgBuf);
	MessageBox(nullptr, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}

funcInitializeAudio LoadModule()
{
	HINSTANCE hDLL = LoadLibrary(L"FmodAudio.dll");

	if (hDLL == nullptr)
	{
		ErrorExit(TEXT("FmodAudio"));
	}
	else
	{
		funcInitializeAudio initModule = reinterpret_cast<funcInitializeAudio>(GetProcAddress(hDLL, "InitializeAudio"));
		if (initModule != nullptr)
		{
			return initModule;
		}
		FreeLibrary(hDLL);
		ErrorExit(TEXT("FmodAudio"));
	}
	return nullptr;
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
	


	// Test audio

	funcInitializeAudio initializeAudio = LoadModule();
	Viper::MemoryManager allocator;
	Viper::AudioManager* manager = initializeAudio(100, allocator);

	manager->SetListener3dAttributes(Viper::Vector3(0, 0, 0), Viper::Vector3(0, 0, 1), Viper::Vector3(0, 1, 0));
	manager->LoadSoundBank("sounds/ZombieWars.bank");
	manager->LoadSoundBank("sounds/ZombieWars.strings.bank");
	manager->LoadSoundBankEvents("sounds/ZombieWars.bank");

	Viper::Vector3 position(0, 0, 0);
	manager->SetEvent3dAttributes("event:/GattlingGun-Fire", position, Viper::Vector3(0, 0, 0));
	manager->PlayEvent("event:/GattlingGun-Fire");





	// Run the message loop.
	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		manager->Update();
		position.z += 0.5;
		manager->SetEvent3dAttributes("event:/GattlingGun-Fire", position, Viper::Vector3(0, 0, 0));
		Sleep(500);
	}

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
