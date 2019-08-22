#pragma once

#include <Windows.h>

//根据窗口句柄获取进程Pid
//WindowName:窗口名
DWORD SearchProcessByWindow(WCHAR* WindowName);

//根据进程快照获取进程Pid
//ProcessName:进程名
DWORD SearchProcessBySnapshot(WCHAR* ProcessName);

//根据Pid注入进程
bool Inject(DWORD Pid, WCHAR* path);

//GetPid
DWORD GetPid();
