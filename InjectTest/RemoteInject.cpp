#include "stdafx.h"
#include "RemoteInject.h"
#include <Windows.h>
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
	DWORD result = WriteProcessMemory((HANDLE)hProcess, VirtualAddress, path, wcslen(path) * 2 + 2, &dwSize);
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