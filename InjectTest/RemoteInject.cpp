#include "stdafx.h"
#include "RemoteInject.h"
#include <Windows.h>
#include <tlhelp32.h>
#define DEBUG 1

//���ݴ��ھ����ȡ����Pid
//�ɹ�����Pid ʧ�ܷ���0
DWORD SearchProcessByWindow(WCHAR* WindowName);
//���ݽ��̿��ջ�ȡ����Pid
//�ɹ�����Pid ʧ�ܷ���0
DWORD SearchProcessBySnapshot(WCHAR* WindowName);
//����Pidע�����
//�ɹ�����1 ʧ�ܷ���0
bool Inject(DWORD Pid, WCHAR* path);
//�������Ƿ�Ϊ�� Ϊ���򷵻�1 ����ӡ������
bool check(HANDLE hThread);

//GetPid
DWORD GetPid();



//���ݴ��ھ����ȡ����Pid
//�ɹ�����Pid ʧ�ܷ���-1
DWORD SearchProcessByWindow(WCHAR* WindowName) {
	//��ʽ1 - Ѱ�Ҵ��ھ��

	//HWND hProWnd = FindWindow(NULL, (LPCSTR)L"D:\\Luluting\\Software\\C++\\MyProjects\\Test1\\Debug\\Target.exe");
	HWND hProWnd = FindWindow(NULL, (LPCWSTR)WindowName);

	if (!hProWnd) {
		printf("�Զ���ȡ���ھ��ʧ�� ������̴������Ƿ�������ȷ\n");
		return 0;
	}
	DWORD Pid = 0;
	GetWindowThreadProcessId(hProWnd, &Pid);
	if (!Pid) {
		printf("��ȡ���̾��ʧ�� ������");
		return 0;
	}
	printf("��ȡ����Pid�ɹ� PidΪ:%d\n", Pid);
	return Pid;
}


//���ݽ��̿��ջ�ȡ����Pid
//�ɹ�����Pid ʧ�ܷ���-1
DWORD SearchProcessBySnapshot(WCHAR* ProcessName) {
	//���̿��վ��
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (check(hSnapshot)) {
		CloseHandle(hSnapshot);
		printf("��������ʧ�� ������\n");
		return 0;
	}
	//���̽ṹ
	DWORD a = sizeof(PROCESSENTRY32);
	PROCESSENTRY32 pe = { sizeof(pe) };
	//��ʼѰ��
	Process32First(hSnapshot, &pe);
	do {
		if (wcscmp((CONST WCHAR*)pe.szExeFile, (CONST WCHAR*)ProcessName) == 0) {
			CloseHandle(hSnapshot);
			printf("�ҵ�Ŀ�� ����PidΪ:%d\n", pe.th32ProcessID);
			return pe.th32ProcessID;
		}

	} while (Process32Next(hSnapshot, &pe));

	CloseHandle(hSnapshot);
	return (DWORD)0;
}

//����Pidע�����
//�ɹ�����1 ʧ�ܷ���0
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
	if (check(VirtualAddress)) {
		CloseHandle(hProcess);
		printf("����ռ�ʧ�� ������\n");
		return 0;
	}
	//���DLL�ļ���ַ
	DWORD result = WriteProcessMemory((HANDLE)hProcess, VirtualAddress, path, wcslen(path) * 2 + 2, NULL);
	if (result == NULL) {
		printf("Զ��д���ַ���ʧ�� ������\n");
		return 0;
	}
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
	if (check(hThread)) {
		CloseHandle(hThread);
		printf("����Զ���߳�ʧ�� ������\n");
		return 0;
	}

	//��Զ���߳�ִ����� �ͷ�����ռ�
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hProcess);
	CloseHandle(hThread);
	VirtualFreeEx(hProcess, VirtualAddress, 1, MEM_COMMIT);

	printf("ע�����\n");
	return 1;
} 

bool check(HANDLE hThread) {
	if (hThread == NULL) {
		DWORD errorcode = GetLastError();
		printf("�������� ������Ϊ%d\n", errorcode);
		return 1;
	}
	return 0;
}

//GetPid
DWORD GetPid() {
	DWORD option;
	printf("***********��ѡ���ȡPid�ķ�ʽ***********\n");
	printf("1.���ݴ��ھ��(��Ҫ���봰����)\n2.���ݽ��̿��ջ�ȡ����Pid(��Ҫ���������)\n3.�ֶ�����Pid\n�����������˳�...\n");
	scanf_s("%d", &option);
	switch (option) {
		DWORD dwID;

	case 1: {
		WCHAR WName[100] = {0};
		printf("������WindowName:");
		wscanf_s(L"%s", WName, 100);
		dwID = SearchProcessByWindow(WName);
		return dwID;
		break;
	}
	case 2: {
		WCHAR PName[100] = {0};
		printf("������ProcessName:");
		wscanf_s(L"%s", PName, 100);
		dwID = SearchProcessBySnapshot(PName);
		return dwID;
		break;
	}
	case 3:
		printf("������Pid:");
		scanf_s("%d", &dwID);
		return dwID;
		break;
	default:
		break;
	}
	return 0;

}