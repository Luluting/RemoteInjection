#include "stdafx.h"
#include "RemoteInject.h"
#include <Windows.h>
#include <tlhelp32.h>

//根据窗口句柄获取进程Pid
DWORD SearchProcessByWindow(WCHAR* WindowName);
//根据进程快照获取进程Pid
DWORD SearchProcessBySnapshot(WCHAR* WindowName);

//根据Pid注入进程
bool Inject(DWORD Pid, WCHAR* path);

//根据窗口句柄获取进程Pid
DWORD SearchProcessByWindow(WCHAR* WindowName) {
	//方式1 - 寻找窗口句柄

	//HWND hProWnd = FindWindow(NULL, (LPCSTR)L"D:\\Luluting\\Software\\C++\\MyProjects\\Test1\\Debug\\Target.exe");
	HWND hProWnd = FindWindow(NULL, (LPCWSTR)WindowName);

	if (!hProWnd) {
		printf("自动获取窗口句柄失败 请检查进程窗口名是否输入正确\n");
		return -1;
	}
	DWORD Pid = 0;
	GetWindowThreadProcessId(hProWnd, &Pid);
	if (!Pid) {
		printf("获取进程句柄失败 请重试");
		return -1;
	}
	return Pid;
}

//根据进程快照获取进程Pid
DWORD SearchProcessBySnapshot(WCHAR* ProcessName) {
	//进程快照句柄
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	//进程结构
	PROCESSENTRY32 pe = {0};
	//开始寻找
	Process32First(hSnapshot, &pe);
	DWORD shit = GetLastError();
	do {
		if (wcscmp((CONST WCHAR*)pe.szExeFile, (CONST WCHAR*)ProcessName) == 0) {
			CloseHandle(hSnapshot);
			return pe.th32ProcessID;
		}

	} while (Process32Next(hSnapshot, &pe));

	CloseHandle(hSnapshot);
	return (DWORD)NULL;
}


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
	DWORD result = WriteProcessMemory((HANDLE)hProcess, VirtualAddress, path, wcslen(path) * 2 + 2, (SIZE_T*)&dwSize);
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

