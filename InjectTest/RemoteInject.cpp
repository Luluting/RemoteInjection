#include "stdafx.h"
#include "RemoteInject.h"
#include <Windows.h>
#include <tlhelp32.h>

//���ݴ��ھ����ȡ����Pid
DWORD SearchProcessByWindow(WCHAR* WindowName);
//���ݽ��̿��ջ�ȡ����Pid
DWORD SearchProcessBySnapshot(WCHAR* WindowName);

//����Pidע�����
bool Inject(DWORD Pid, WCHAR* path);

//���ݴ��ھ����ȡ����Pid
DWORD SearchProcessByWindow(WCHAR* WindowName) {
	//��ʽ1 - Ѱ�Ҵ��ھ��

	//HWND hProWnd = FindWindow(NULL, (LPCSTR)L"D:\\Luluting\\Software\\C++\\MyProjects\\Test1\\Debug\\Target.exe");
	HWND hProWnd = FindWindow(NULL, (LPCWSTR)WindowName);

	if (!hProWnd) {
		printf("�Զ���ȡ���ھ��ʧ�� ������̴������Ƿ�������ȷ\n");
		return -1;
	}
	DWORD Pid = 0;
	GetWindowThreadProcessId(hProWnd, &Pid);
	if (!Pid) {
		printf("��ȡ���̾��ʧ�� ������");
		return -1;
	}
	return Pid;
}

//���ݽ��̿��ջ�ȡ����Pid
DWORD SearchProcessBySnapshot(WCHAR* ProcessName) {
	//���̿��վ��
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	//���̽ṹ
	PROCESSENTRY32 pe = {0};
	//��ʼѰ��
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
	//��ȡ���̾��
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Pid);
	//���½��̿���һ������ռ�
	LPVOID VirtualAddress = VirtualAllocEx(
		hProcess,
		NULL,
		1,
		MEM_COMMIT,
		PAGE_READWRITE
	);
	//���DLL�ļ���ַ
	DWORD dwSize = 0;
	DWORD result = WriteProcessMemory((HANDLE)hProcess, VirtualAddress, path, wcslen(path) * 2 + 2, (SIZE_T*)&dwSize);
	//����Զ���߳� ����LoadLibrary
	HANDLE hThread = CreateRemoteThread(
		hProcess,
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)LoadLibrary,
		VirtualAddress,
		NULL,
		NULL
	);
	//��Զ���߳�ִ����� �ͷ�����ռ�
	WaitForSingleObject(hThread, INFINITE);
	VirtualFreeEx(hProcess, VirtualAddress, 1, MEM_COMMIT);
	return 0;
}

