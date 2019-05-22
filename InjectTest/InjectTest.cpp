// InjectTest.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include "RemoteInject.h";

//要注入的DLL名字
#define path _T("InjectDll.dll")

int main(){
	//获取进程Pid
	DWORD dwID;
	printf("获取Pid\n");

	//方式1 - 窗口句柄检索
	//dwID = SearchProcessByWindow((WCHAR*)L"WndName");
	//方式2 - 进程快照检索
	dwID = SearchProcessBySnapshot((WCHAR*)L"Calculator.exe");
	//方式3 - 手动输入Pid
	//printf("请输入Pid:");
	//scanf_s("%d", &dwID);
	if (dwID == 0) {
		printf("获取Pid失败 请重试\n");
		return -1;
	}
	printf("获取Pid完毕 开始注入\n");
	
	Inject(dwID, (WCHAR*)path);

    return 0;
}

