// InjectDll.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "InjectDll.h"


extern "C" _declspec(dllexport) int add(int x, int y) {
	return x + y;
}

extern "C" _declspec(dllexport) int sub(int x, int y) {
	return x - y;
}


