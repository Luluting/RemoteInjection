// InjectTest.cpp: 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include <Windows.h>
#include "RemoteInject.h";
#define path _T("InjectDll.dll")


typedef int(*pfun)(int x, int y);



int main(){
	//HINSTANCE hDLL = (HINSTANCE)LoadLibrary((LPCWSTR)L"InjectDll.dll");
	//	
	//pfun add = (pfun)GetProcAddress(hDLL, (LPCSTR)"add");

	//if (add) {
	//	printf("%d\n", add(1, 2));
	//}
	
	//获取进程Pid
	DWORD dwID;
	printf("获取Pid\n");
	//选择方式1
	HWND hProWnd = FindWindow(NULL, L"D:\\Luluting\\Software\\C++\\MyProjects\\Test1\\Debug\\Target.exe");
	if (!hProWnd) {
		printf("自动获取窗口句柄失败 请检查进程窗口名是否输入正确\n");
		getchar();
		return -1;
	}
	DWORD Pid = 0;
	GetWindowThreadProcessId(hProWnd, &Pid);

	//选择方式2
	//scanf_s("%d", &dwID);
	
	if (!Pid) {
		printf("获取进程句柄失败 请重试");
		getchar();
		return -1;
	}
	printf("获取Pid完毕 开始注入\n");
	
	Inject(Pid, (WCHAR*)path);
	

	getchar();
    return 0;
}

