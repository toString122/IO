#pragma once
#include <Windows.h>
#include <stdio.h>
#include <Shlwapi.h>
#include <tchar.h>

typedef struct  TongL_Process
{
	ULONG pid;						//����ID
	ULONG64 Address;				//�ڴ��ַ
	ULONG64 buf;					//������ָ��
	ULONG Size;					//�ڴ��С
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



//��������
ULONG ReadInt(ULONG pid, ULONG64 address);
//���ֽ���
BYTE ReadByte(ULONG pid, ULONG64 address);
//����������
SHORT ReadShort(ULONG pid, ULONG64 address);
//����������
INT64 ReadInt64(ULONG pid, ULONG64 address);
//��С����
FLOAT ReadFloat(ULONG pid, ULONG64 address);
//���ֽڼ�
BOOL ReadBytes(ULONG pid, ULONG64 address, PBYTE Buffer, ULONG size);
//���ı���
const char* ReadText(ULONG pid, ULONG64 address, ULONG size);

//д�ֽ���
BOOL WriteByte(ULONG pid, ULONG64 address, BYTE Buffer);
//д��������
BOOL WriteShort(ULONG pid, ULONG64 address, SHORT Buffer);
//д������
BOOL WriteInt(ULONG pid, ULONG64 address, INT Buffer);
//д��������
BOOL WriteInt64(ULONG pid, ULONG64 address, INT64 Buffer);
//дС����
BOOL WriteFloat(ULONG pid, ULONG64 address, FLOAT Buffer);
//д�ֽڼ�
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