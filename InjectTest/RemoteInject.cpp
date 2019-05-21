#include "stdafx.h"
#include "RemoteInject.h"
#include <Windows.h>
bool Inject(DWORD Pid, WCHAR* path) {
	//获取进程句柄
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Pid);
	//在新进程开辟一个虚拟空间
	LPVOID VirtualAddress = VirtualAllocEx(
		hProcess,
		NULL,
		1,
		MEM_COMMIT,
		PAGE_READWRITE
	);
	//存放DLL文件地址
	DWORD dwSize = 0;
	DWORD result = WriteProcessMemory((HANDLE)hProcess, VirtualAddress, path, wcslen(path) * 2 + 2, &dwSize);
	//创建远程线程 调用LoadLibrary
	HANDLE hThread = CreateRemoteThread(
		hProcess,
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)LoadLibrary,
		VirtualAddress,
		NULL,
		NULL
	);
	//等远程线程执行完毕 释放虚拟空间
	WaitForSingleObject(hThread, INFINITE);
	VirtualFreeEx(hProcess, VirtualAddress, 1, MEM_COMMIT);
	return 0;
}