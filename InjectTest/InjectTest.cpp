// InjectTest.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include "RemoteInject.h";

#define DEBUG 0

//准备注入的DLL名字 or 路径
#define path (L"InjectDll.dll")

int main(){

#if DEBUG == 1
	DWORD dwID;
	//方式1 - 窗口句柄检索
	//dwID = SearchProcessByWindow((WCHAR*)L"D:\\Luluting\\Project\\Github\\Luluting\\RemoteInjection\\Debug\\Target.exe");
	//方式2 - 进程快照检索
	//dwID = SearchProcessBySnapshot((WCHAR*)L"Target.exe");
	//方式3 - 手动输入Pid
	printf("请输入Pid:");
	scanf_s("%d", &dwID);
#endif

	//获取进程Pid
	DWORD dwID = GetPid();

	if (dwID == NULL) {
		printf("PidEnd\n");
		return 0;
	}
	//获取Pid完毕 开始注入
	rewind(stdin);
	printf("Enter to Inject\n");
	getchar();
	//开始注入
	Inject(dwID, (WCHAR*)path);
	
	system("pause");
    return 0;
}

