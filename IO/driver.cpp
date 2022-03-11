#include"driver.h"

HANDLE Handle;




// ��װ����
BOOL installDvr(CONST WCHAR drvPath[50], CONST WCHAR serviceName[20]) {

    // �򿪷�����ƹ��������ݿ�
    SC_HANDLE schSCManager = OpenSCManager(
        NULL,                   // Ŀ������������,NULL�����ӱ��ؼ�����ϵķ�����ƹ�����
        NULL,                   // ������ƹ��������ݿ�����ƣ�NULL���� SERVICES_ACTIVE_DATABASE ���ݿ�
        SC_MANAGER_ALL_ACCESS   // ����Ȩ��
    );
    if (schSCManager == NULL) {
        CloseServiceHandle(schSCManager);
        return FALSE;
    }

    // ����������������������ƹ��������ݿ�
    SC_HANDLE schService = CreateService(
        schSCManager,               // ����ؼ����������ݿ�ľ��
        serviceName,                // Ҫ��װ�ķ��������
        serviceName,                // �û��������������ʶ�������ʾ����
        SERVICE_ALL_ACCESS,         // �Է���ķ���Ȩ�ޣ�����ȫȨ��
        SERVICE_KERNEL_DRIVER,      // �������ͣ���������
        SERVICE_DEMAND_START,       // ��������ѡ����̵��� StartService ʱ����
        SERVICE_ERROR_IGNORE,       // ����޷����������Դ����������
        drvPath,                    // �����ļ�����·������������ո���Ҫ���˫����
        NULL,                       // ���������ĸ��ض����飺��������ĳ����
        NULL,                       // ���ն�����Ψһ���ֵ��������
        NULL,                       // �������˳�����飺����û��������
        NULL,                       // ���з�����˻�����ʹ�� LocalSystem �˻�
        NULL                        // LocalSystem �˻�����
    );
    if (schService == NULL) {
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return FALSE;
    }

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return TRUE;
}

// ��������
BOOL startDvr(CONST WCHAR serviceName[20]) {

    // �򿪷�����ƹ��������ݿ�
    SC_HANDLE schSCManager = OpenSCManager(
        NULL,                   // Ŀ������������,NULL�����ӱ��ؼ�����ϵķ�����ƹ�����
        NULL,                   // ������ƹ��������ݿ�����ƣ�NULL���� SERVICES_ACTIVE_DATABASE ���ݿ�
        SC_MANAGER_ALL_ACCESS   // ����Ȩ��
    );
    if (schSCManager == NULL) {
        CloseServiceHandle(schSCManager);
        return FALSE;
    }

    // �򿪷���
    SC_HANDLE hs = OpenService(
        schSCManager,           // ����ؼ����������ݿ�ľ��
        serviceName,            // Ҫ�򿪵ķ�����
        SERVICE_ALL_ACCESS      // �������Ȩ�ޣ�����Ȩ��
    );
    if (hs == NULL) {
        CloseServiceHandle(hs);
        CloseServiceHandle(schSCManager);
        return FALSE;
    }
    if (StartService(hs, 0, 0) == 0) {
        CloseServiceHandle(hs);
        CloseServiceHandle(schSCManager);
        return FALSE;
    }


    CloseServiceHandle(hs);
    CloseServiceHandle(schSCManager);
    return TRUE;
}

// ֹͣ����
BOOL stopDvr(CONST WCHAR serviceName[20]) {

    // �򿪷�����ƹ��������ݿ�
    SC_HANDLE schSCManager = OpenSCManager(
        NULL,                   // Ŀ������������,NULL�����ӱ��ؼ�����ϵķ�����ƹ�����
        NULL,                   // ������ƹ��������ݿ�����ƣ�NULL���� SERVICES_ACTIVE_DATABASE ���ݿ�
        SC_MANAGER_ALL_ACCESS   // ����Ȩ��
    );
    if (schSCManager == NULL) {
        CloseServiceHandle(schSCManager);
        return FALSE;
    }

    // �򿪷���
    SC_HANDLE hs = OpenService(
        schSCManager,           // ����ؼ����������ݿ�ľ��
        serviceName,            // Ҫ�򿪵ķ�����
        SERVICE_ALL_ACCESS      // �������Ȩ�ޣ�����Ȩ��
    );
    if (hs == NULL) {
        CloseServiceHandle(hs);
        CloseServiceHandle(schSCManager);
        return FALSE;
    }

    // ���������������
    SERVICE_STATUS status;
    if (QueryServiceStatus(hs, &status) == 0) {
        CloseServiceHandle(hs);
        CloseServiceHandle(schSCManager);
        return FALSE;
    }
    if (status.dwCurrentState != SERVICE_STOPPED &&
        status.dwCurrentState != SERVICE_STOP_PENDING
        ) {
        // ���͹رշ�������
        if (ControlService(
            hs,                         // ������
            SERVICE_CONTROL_STOP,       // �����룺֪ͨ����Ӧ��ֹͣ
            &status                     // �������µķ���״̬��Ϣ
        ) == 0) {
            CloseServiceHandle(hs);
            CloseServiceHandle(schSCManager);
            return FALSE;
        }

        // �жϳ�ʱ
        INT timeOut = 0;
        while (status.dwCurrentState != SERVICE_STOPPED) {
            timeOut++;
            QueryServiceStatus(hs, &status);
            Sleep(50);
        }
        if (timeOut > 80) {
            CloseServiceHandle(hs);
            CloseServiceHandle(schSCManager);
            return FALSE;
        }
    }

    CloseServiceHandle(hs);
    CloseServiceHandle(schSCManager);
    return TRUE;
}

// ж������
BOOL unloadDvr(CONST WCHAR serviceName[20]) {

    // �򿪷�����ƹ��������ݿ�
    SC_HANDLE schSCManager = OpenSCManager(
        NULL,                   // Ŀ������������,NULL�����ӱ��ؼ�����ϵķ�����ƹ�����
        NULL,                   // ������ƹ��������ݿ�����ƣ�NULL���� SERVICES_ACTIVE_DATABASE ���ݿ�
        SC_MANAGER_ALL_ACCESS   // ����Ȩ��
    );
    if (schSCManager == NULL) {
        CloseServiceHandle(schSCManager);
        return FALSE;
    }

    // �򿪷���
    SC_HANDLE hs = OpenService(
        schSCManager,           // ����ؼ����������ݿ�ľ��
        serviceName,            // Ҫ�򿪵ķ�����
        SERVICE_ALL_ACCESS      // �������Ȩ�ޣ�����Ȩ��
    );
    if (hs == NULL) {
        CloseServiceHandle(hs);
        CloseServiceHandle(schSCManager);
        return FALSE;
    }

    // ɾ������
    if (DeleteService(hs) == 0) {
        CloseServiceHandle(hs);
        CloseServiceHandle(schSCManager);
        return FALSE;
    }

    CloseServiceHandle(hs);
    CloseServiceHandle(schSCManager);
    return TRUE;
}









void ReadWriteBegin()
{
	Handle = CreateFileW(L"\\\\.\\RW64", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
}

void ReadWriteEnd()
{
	CloseHandle(Handle);
}

BOOL ReadMemory(ULONG64 pid, ULONG64 address, PBYTE Buffer, ULONG size) {
	TongL_PROCESS data = { 0 };
	data.pid = pid;
	data.Address = address;
	data.buf = (ULONG64)Buffer;
	data.Size = size;


	DWORD dwSize = 0;
    ReadWriteBegin();
	BOOL ret = DeviceIoControl(Handle, ID_Read, &data, sizeof(data), &data, sizeof(data), &dwSize, NULL);
    ReadWriteEnd();
	return ret;
}

BOOL WriteMemory(ULONG64 pid, ULONG64 address, PBYTE Buffer, ULONG size) {

    MyWrite data;
    data.ProcessID = pid;
    data.pSource = (ULONG64)Buffer;
    data.pTarget = address;
    data.Size = size;

    DWORD dwSize = 0;
    ReadWriteBegin();
    BOOL ret = DeviceIoControl(Handle, ID_Write, &data, sizeof(data), &data, sizeof(data), &dwSize, NULL);
    ReadWriteEnd();
    return ret;
}

ULONG64 GetModuleAddress(ULONG pid, LPCWSTR ModuleName) {
    TongL_PROCESS data = { 0 };
    WCHAR buf[MAX_PATH] = { 0 };
    data.pid = pid;
    data.buf = (ULONG64)buf;
    //data.Size = 7;
    DWORD dwSize = 0;
    if (ModuleName != NULL)
        wcscpy_s(buf, ModuleName);
    ReadWriteBegin();
    BOOL ret = DeviceIoControl(Handle, ID_GetModel, &data, sizeof(data), &data, sizeof(data), &dwSize, NULL);
    ReadWriteEnd();
    return *(ULONG64*)&buf;
}

BOOL ProtectProcess(ULONG pid) {
    TongL_PROCESS data = { 0 };
    data.pid = pid;
    DWORD dwSize = 0;
    ReadWriteBegin();
    BOOL ret = DeviceIoControl(Handle, ID_MyProtect, &data, sizeof(data), &data, sizeof(data), &dwSize, NULL);
    ReadWriteEnd();
    return ret;
}




ULONG ReadInt(ULONG pid, ULONG64 address)
{
	INT buf = 0;
    
	ReadMemory(pid, address, (PBYTE)&buf, sizeof(INT));
    
	return buf;
}

BYTE ReadByte(ULONG pid, ULONG64 address)
{
    BYTE buf = '\0';
    ReadMemory(pid, address, (PBYTE)&buf, sizeof(BYTE));
    return buf;
}
SHORT ReadShort(ULONG pid, ULONG64 address)
{
    SHORT buf = 0;
    ReadMemory(pid, address, (PBYTE)&buf, sizeof(SHORT));
    return buf;
}
INT64 ReadInt64(ULONG pid, ULONG64 address)
{
    INT64 buf = 0;
    ReadMemory(pid, address, (PBYTE)&buf, sizeof(INT64));
    return buf;
}
FLOAT ReadFloat(ULONG pid, ULONG64 address)
{
    FLOAT buf = 0.0f;
    ReadMemory(pid, address, (PBYTE)&buf, sizeof(FLOAT));
    return buf;
}
BOOL ReadBytes(ULONG pid, ULONG64 address, PBYTE Buffer, ULONG size)
{
    return ReadMemory(pid, address, Buffer, size);
}
const char* ReadText(ULONG pid, ULONG64 address, ULONG size) {
    PBYTE buf = new BYTE[size];
    ReadBytes(pid, address, buf, size);
    char* p = new char[size];
    memcpy(p, buf, size);
    p[size] = 0;
    return p;
}



BOOL WriteByte(ULONG pid, ULONG64 address, BYTE Buffer)
{
    return WriteMemory(pid, address, (PBYTE)&Buffer, sizeof(BYTE));
}
BOOL WriteShort(ULONG pid, ULONG64 address, SHORT Buffer)
{
    return WriteMemory(pid, address, (PBYTE)&Buffer, sizeof(SHORT));
}
BOOL WriteInt(ULONG pid, ULONG64 address, INT Buffer)
{
    return WriteMemory(pid, address, (PBYTE)&Buffer, sizeof(INT));
}
BOOL WriteInt64(ULONG pid, ULONG64 address, INT64 Buffer)
{
    return WriteMemory(pid, address, (PBYTE)&Buffer, sizeof(INT64));
}
BOOL WriteFloat(ULONG pid, ULONG64 address, FLOAT Buffer)
{
    return WriteMemory(pid, address, (PBYTE)&Buffer, sizeof(FLOAT));
}
BOOL WriteBytes(ULONG pid, ULONG64 address, PBYTE Buffer, ULONG size)
{
    return WriteMemory(pid, address, Buffer, size);
}
