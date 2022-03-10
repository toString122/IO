#include <iostream>
#include <Windows.h>
#include "driver.h"
using namespace std;


int main()
{
    if (installDvr(L"C:\\hb.sys", L"LYSM_service") == TRUE) {
        cout << "安装成功" << endl;
    }
    if (startDvr(L"LYSM_service") == TRUE) {
        cout << "启动成功." << endl;
    }
    //读整数型内存
    ULONG pid = 8232;

    ULONG Read_Int = ReadInt(pid, 4880096L);
   
    printf("读整数型内存:%u\n", Read_Int);

    WriteInt(pid, 4880096L,10);

    ULONG Read_Int1 = ReadInt(pid, 4880096L);

    printf("读整数型内存:%u\n", Read_Int1);


    ULONG64 Address = GetModuleAddress(pid, L"123.exe");
    printf("123.exe模块地址:%llX\n", Address);

    if (stopDvr(L"LYSM_service") == TRUE) {
        cout << "停止成功" << endl;
    }
    if (unloadDvr(L"LYSM_service") == TRUE) {
        cout << "卸载成功." << endl;
    }


    getchar();
    return 0;
}
