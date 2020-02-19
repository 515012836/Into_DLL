#include <stdio.h>
#include <Windows.h>

typedef void(*PFN_DLL)();//函数指针

int main(void) {
	//1.加载我们的DLL到本进程
	//返回一个模块句柄
	//句柄:一种特殊的值,通过它来找到对应的资源
	//需要先开发好自己的DLL
	HMODULE hDLL = LoadLibraryA("hackerDLL.dll");
	if (hDLL == NULL) {
		printf("加载DLL文件失败!失败原因:[%d]\n",GetLastError());
		return 1;
	}
	
	//调用安装钩子的函数
	PFN_DLL HookStart = (PFN_DLL)GetProcAddress(
		hDLL,//DLL(动态库)的模块句柄
		"HookStart");//函数名使用"字符串"!!
	PFN_DLL HookStop = (PFN_DLL)GetProcAddress(hDLL, "HookStop");
	HookStart();//安装钩子

	system("pause");

	//卸载钩子
	HookStop();

	//释放指定的动态链接库
	FreeLibrary(hDLL);
	
	system("pause");
	return 0;
}