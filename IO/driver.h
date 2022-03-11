#pragma once
#include <Windows.h>
#include <stdio.h>
#include <Shlwapi.h>
#include <tchar.h>

typedef struct  TongL_Process
{
	ULONG pid;						//进程ID
	ULONG64 Address;				//内存地址
	ULONG64 buf;					//缓冲区指针
	ULONG Size;					//内存大小
}TongL_PROCESS, * PTongL_PROCESS;


typedef struct mywrite
{
	ULONG64 ProcessID;
	ULONG64 pSource;
	ULONG64 pTarget;
	ULONG64 Size;
} MyWrite, * PMyWrite;





#define ID_GetModel	   CTL_CODE(FILE_DEVICE_UNKNOWN, 0x6665, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define ID_Read	   CTL_CODE(FILE_DEVICE_UNKNOWN, 0x6666, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define ID_Write	   CTL_CODE(FILE_DEVICE_UNKNOWN, 0x6667, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define ID_MyProtect   CTL_CODE(FILE_DEVICE_UNKNOWN, 0x6668, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define ID_MyHide     CTL_CODE(FILE_DEVICE_UNKNOWN, 0x6669, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)



//读整数型
ULONG ReadInt(ULONG pid, ULONG64 address);
//读字节型
BYTE ReadByte(ULONG pid, ULONG64 address);
//读短整数型
SHORT ReadShort(ULONG pid, ULONG64 address);
//读长整数型
INT64 ReadInt64(ULONG pid, ULONG64 address);
//读小数型
FLOAT ReadFloat(ULONG pid, ULONG64 address);
//读字节集
BOOL ReadBytes(ULONG pid, ULONG64 address, PBYTE Buffer, ULONG size);
//读文本型
const char* ReadText(ULONG pid, ULONG64 address, ULONG size);

//写字节型
BOOL WriteByte(ULONG pid, ULONG64 address, BYTE Buffer);
//写短整数型
BOOL WriteShort(ULONG pid, ULONG64 address, SHORT Buffer);
//写整数型
BOOL WriteInt(ULONG pid, ULONG64 address, INT Buffer);
//写长整数型
BOOL WriteInt64(ULONG pid, ULONG64 address, INT64 Buffer);
//写小数型
BOOL WriteFloat(ULONG pid, ULONG64 address, FLOAT Buffer);
//写字节集
BOOL WriteBytes(ULONG pid, ULONG64 address, PBYTE Buffer, ULONG size);

BOOL WriteMemory(ULONG64 pid, ULONG64 address, PBYTE Buffer, ULONG size);
BOOL ReadMemory(ULONG64 pid, ULONG64 address, PBYTE Buffer, ULONG size);

void ReadWriteBegin();
void ReadWriteEnd();
BOOL ProtectProcess(ULONG pid);
ULONG64 GetModuleAddress(ULONG pid, LPCWSTR ModuleName);
BOOL installDvr(CONST WCHAR drvPath[50], CONST WCHAR serviceName[20]);
BOOL startDvr(CONST WCHAR serviceName[20]);
BOOL stopDvr(CONST WCHAR serviceName[20]);
BOOL unloadDvr(CONST WCHAR serviceName[20]);