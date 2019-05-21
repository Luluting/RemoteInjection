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
		MessageBox(NULL, (LPCWSTR)L"ProcessCome", (LPCWSTR)L"Attached", MB_OK);
    case DLL_THREAD_ATTACH:
		MessageBox(NULL, (LPCWSTR)L"ThreadCome", (LPCWSTR)L"Attached", MB_OK);
    case DLL_THREAD_DETACH:
		MessageBox(NULL, (LPCWSTR)L"ThreadGo", (LPCWSTR)L"Leave", MB_OK);
    case DLL_PROCESS_DETACH:
		MessageBox(NULL, (LPCWSTR)L"ProcessGo", (LPCWSTR)L"LEAVE", MB_OK);
        break;
    }
    return TRUE;
}

