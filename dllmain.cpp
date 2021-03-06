#include "stdafx.h"
#include "d3d11hook.h"
#include <iostream>

char dlldir[512];
char* GetDirectoryFile(char* filename)
{
	static char path[512];
	strcpy_s(path, dlldir);
	strcat_s(path, filename);
	return path;
}

DWORD WINAPI MainThread(HMODULE hModule)
{
	DisableThreadLibraryCalls(hModule);

	GetModuleFileNameA(hModule, dlldir, 512);

	for (int i = (int)strlen(dlldir); i > 0; i--)
	{
		if (dlldir[i] == '\\')
		{
			dlldir[i + 1] = 0;
			break;
		}
	}

	ImplHookDX11_Init(hModule, dlldir);

	return S_OK;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, hModule, NULL, NULL);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
