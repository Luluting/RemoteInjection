#pragma once

#include <Windows.h>

//���ݴ��ھ����ȡ����Pid
//WindowName:������
DWORD SearchProcessByWindow(WCHAR* WindowName);

//���ݽ��̿��ջ�ȡ����Pid
//ProcessName:������
DWORD SearchProcessBySnapshot(WCHAR* ProcessName);

//����Pidע�����
bool Inject(DWORD Pid, WCHAR* path);

//GetPid
DWORD GetPid();
