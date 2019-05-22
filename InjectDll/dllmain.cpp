// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <Windows.h>
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		MessageBox(NULL, (LPCSTR)L"ProcessCome", (LPCSTR)L"Attached", MB_OK);
		break;
    case DLL_THREAD_ATTACH:
		MessageBox(NULL, (LPCSTR)L"ThreadCome", (LPCSTR)L"Attached", MB_OK);
		break;
	case DLL_THREAD_DETACH:
		MessageBox(NULL, (LPCSTR)L"ThreadGo", (LPCSTR)L"Leave", MB_OK);
		break;
	case DLL_PROCESS_DETACH:
		MessageBox(NULL, (LPCSTR)L"ProcessGo", (LPCSTR)L"LEAVE", MB_OK);
        break;
    }
    return TRUE;
}

