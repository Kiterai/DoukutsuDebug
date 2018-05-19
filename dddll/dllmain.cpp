// dllmain.cpp : DLL アプリケーションのエントリ ポイントを定義します。
#include "stdafx.h"
#include <TlHelp32.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

struct CSData
{
	int MyCharX;
	int MyCharY;
	int MyCharVelX;
	int MyCharVelY;
	int Frame;
};

DLLEXPORT HANDLE FindCaveStory()
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnapShot == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}

	PROCESSENTRY32 process;

	process.dwSize = sizeof(process);

	Process32First(hSnapShot, &process);

	bool flag = false;

	do {
		TCHAR buf[512];
		_tcscpy_s(buf, process.szExeFile);	// TO FIX
		PathStripPath(buf);

		if (_tcscmp(buf, _T("Doukutsu.exe")) == 0)
		{
			flag = true;
			break;
		}
		if (_tcscmp(buf, _T("doukutsu.exe")) == 0)
		{
			flag = true;
			break;
		}
	} while (Process32Next(hSnapShot, &process));

	if (!flag)
	{
		return NULL;
	}

	return OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process.th32ProcessID);
}

DLLEXPORT void GetCSData(HANDLE hPrc, CSData* dat)
{
	ReadProcessMemory(hPrc, reinterpret_cast<void*>(0x49E654), &dat->MyCharX, sizeof(dat->MyCharX), NULL);
	ReadProcessMemory(hPrc, reinterpret_cast<void*>(0x49E658), &dat->MyCharY, sizeof(dat->MyCharY), NULL);
	ReadProcessMemory(hPrc, reinterpret_cast<void*>(0x49E66C), &dat->MyCharVelX, sizeof(dat->MyCharVelX), NULL);
	ReadProcessMemory(hPrc, reinterpret_cast<void*>(0x49E670), &dat->MyCharVelY, sizeof(dat->MyCharVelY), NULL);
	ReadProcessMemory(hPrc, reinterpret_cast<void*>(0x49E1EC), &dat->Frame, sizeof(dat->Frame), NULL);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

