// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include<stdlib.h>
#include <stdio.h>
#include <Windows.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		MessageBox(NULL, (LPCWSTR)L"注入成功", (LPCWSTR)L"DLL Injection", MB_OK);
		system("start calc");
		printf("\n注入成功!\n");
		//ShellExecuteA(0, "open", "C:\\Windows\\System32\\calc.exe", 0, 0, 1);
		break;
    case DLL_THREAD_ATTACH:
		//MessageBox(NULL, (LPCWSTR)L"ThreadCome", (LPCWSTR)L"Attached", MB_OK);
		break;
	case DLL_THREAD_DETACH:
		//MessageBox(NULL, (LPCWSTR)L"ThreadGo", (LPCWSTR)L"Leave", MB_OK);
		break;
	case DLL_PROCESS_DETACH:
		//MessageBox(NULL, (LPCWSTR)L"ProcessGo", (LPCWSTR)L"LEAVE", MB_OK);
        break;
    }
    return TRUE;
}

