#include "stdafx.h"
#include "RemoteInject.h"
#include <Windows.h>
#include <tlhelp32.h>

//根据窗口句柄获取进程Pid
//成功返回Pid 失败返回0
DWORD SearchProcessByWindow(WCHAR* WindowName);
//根据进程快照获取进程Pid
//成功返回Pid 失败返回0
DWORD SearchProcessBySnapshot(WCHAR* WindowName);
//根据Pid注入进程
//成功返回1 失败返回0
bool Inject(DWORD Pid, WCHAR* path);
//检查对象是否为空 为空则返回1 并打印错误码
bool check(HANDLE hThread);



//根据窗口句柄获取进程Pid
//成功返回Pid 失败返回-1
DWORD SearchProcessByWindow(WCHAR* WindowName) {
	//方式1 - 寻找窗口句柄

	//HWND hProWnd = FindWindow(NULL, (LPCSTR)L"D:\\Luluting\\Software\\C++\\MyProjects\\Test1\\Debug\\Target.exe");
	HWND hProWnd = FindWindow(NULL, (LPCWSTR)WindowName);

	if (!hProWnd) {
		printf("自动获取窗口句柄失败 请检查进程窗口名是否输入正确\n");
		return 0;
	}
	DWORD Pid = 0;
	GetWindowThreadProcessId(hProWnd, &Pid);
	if (!Pid) {
		printf("获取进程句柄失败 请重试");
		return 0;
	}
	return Pid;
}

//根据进程快照获取进程Pid
//成功返回Pid 失败返回-1
DWORD SearchProcessBySnapshot(WCHAR* ProcessName) {
	//进程快照句柄
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (check(hSnapshot)) {
		CloseHandle(hSnapshot);
		printf("快照拍摄失败 请重试\n");
		return 0;
	}
	//进程结构
	DWORD a = sizeof(PROCESSENTRY32);
	PROCESSENTRY32 pe = { sizeof(pe) };
	//开始寻找
	Process32First(hSnapshot, &pe);
	do {
		if (wcscmp((CONST WCHAR*)pe.szExeFile, (CONST WCHAR*)ProcessName) == 0) {
			CloseHandle(hSnapshot);
			return pe.th32ProcessID;
		}

	} while (Process32Next(hSnapshot, &pe));

	CloseHandle(hSnapshot);
	return (DWORD)0;
}

//根据Pid注入进程
//成功返回1 失败返回0
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
	if (check(VirtualAddress)) {
		CloseHandle(hProcess);
		printf("分配空间失败 请重试\n");
		return 0;
	}
	//存放DLL文件地址
	DWORD result = WriteProcessMemory((HANDLE)hProcess, VirtualAddress, path, wcslen(path) * 2 + 2, NULL);
	if (result == NULL) {
		printf("远程写入字符串失败 请重试\n");
		return 0;
	}
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
	if (check(hThread)) {
		CloseHandle(hThread);
		printf("创建远程线程失败 请重试\n");
		return 0;
	}

	//等远程线程执行完毕 释放虚拟空间
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hProcess);
	CloseHandle(hThread);
	VirtualFreeEx(hProcess, VirtualAddress, 1, MEM_COMMIT);

	printf("注入完成\n");
	return 1;
} 



bool check(HANDLE hThread) {
	if (hThread == NULL) {
		DWORD errorcode = GetLastError();
		printf("发生错误 错误码为%d\n", errorcode);
		return 1;
	}
	return 0;
}