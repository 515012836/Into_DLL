// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

HINSTANCE g_hInstance = NULL;//实例句柄
HHOOK g_hHook = NULL;//钩子句柄

//动态链接库的入口
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
        //加载到其他进程时
    case DLL_PROCESS_ATTACH:
        //写我们需要的功能代码:
        g_hInstance = hModule;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
LRESULT CALLBACK KeyBoardProc(
    int nCode,//钩子代码
    WPARAM wParam,//用户按键"虚拟按键值"
    LPARAM IParam){
    char szPath[MAX_PATH] = { 0, };
    char* p = NULL;

    //判断现在插入的程序,是不是你想攻击的程序
    if (nCode >= 0) {
        //bit 31:0=>press, 1=>release
        //按键按下时,IParam的最高位是0,按键释放时,IParam的最高位是1;
        if (!(IParam & 0x80000000)) {
            //获取当前运行程序的绝对路径
            GetModuleFileNameA(
                NULL,//返回该当前应用程序全路径
                szPath,//存放路径的数组
                MAX_PATH);//存放路径的数组的最大长度
            //查找字符在指定字符串从右边开始的第一次出现的位置,如果成功,
            //即从完整路径中获取应用程序的名称
            p = strrchr(szPath, '\\');
            //查找被注入的程序是不是"记事本"
            if (!_stricmp(p + 1, "nottepad.exe"))//用于对两个字符串进行比较(字符不分大小写)
                return 1;
        }
    }
    //因为其他程序也可能被注入该DLL,如果不是记事本,就把消息传递给当前钩子链中的下一个钩子
    return CallNextHookEx(g_hHook, nCode, wParam, IParam);

}
//实现钩子功能
//__declspec(dllexport)表示这个函数,用来给其他进程调用
extern "C"//兼容C语言,方便找到这个函数,C语言不会改变函数的名称
__declspec(dllexport) void HookStart() {
    //安装钩子
    //把参数2(KeyboardProc),安装键盘钩子,即添加到键盘钩链,实现DLL消息注入
    //此后,只要发生了键盘输入事件,操作系统就会强制把该DLL注入到相应的进程
      g_hHook = SetWindowsHookEx(
        WH_KEYBOARD,//键盘钩子
        KeyBoardProc,//钩子子程,相应的自动执行代码
        g_hInstance, //实例句柄,表示那个动态(DLL)实例
        0);
}

//卸载钩子
__declspec(dllexport) void HookStop() {
    if (g_hHook) {
        UnhookWindowsHookEx(g_hHook);
        g_hHook = NULL;
    }
}

