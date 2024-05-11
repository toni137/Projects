#include <iostream>
#include <Windows.h>

#define dwLocalPlayer 0xDE6964
#define dwForceJump 0x52B7BCC


DWORD WINAPI MainThread(HMODULE hModule)
{
	//Create Console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	std::cout << "Welcome\n";

	uintptr_t moduleBase = (DWORD)GetModuleHandle("client.dll");

	while (!GetAsyncKeyState(VK_END))
	{
		uintptr_t LocalPlayer = *(uintptr_t*)(moduleBase + dwLocalPlayer);
		float* z_coord = (float*)(LocalPlayer + 0x140);
		float* x_coord = (float*)(LocalPlayer + 0x138);
		float* y_coord = (float*)(LocalPlayer + 0x13C);
		int* spin = (int*)(moduleBase + dwForceJump);

		if (GetAsyncKeyState('X'))
		{
			*z_coord = *z_coord + 10.0;
		}
		if (GetAsyncKeyState('C'))
		{
			*x_coord = *x_coord + 10.0;
		}
		if (GetAsyncKeyState('B'))
		{
			*y_coord = *y_coord + 10.0;
		}

		if (GetAsyncKeyState('N'))
		{
			*x_coord = *x_coord - 10.0;
		}
		if (GetAsyncKeyState('L'))
		{
			*y_coord = *y_coord - 10.0;
		}
		if (GetAsyncKeyState('J'))
		{
			*spin = 6;
		}
		
	}
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr));
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
}