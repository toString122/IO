#include"driver.h"

HANDLE Handle;




// 安装驱动
BOOL installDvr(CONST WCHAR drvPath[50], CONST WCHAR serviceName[20]) {

    // 打开服务控制管理器数据库
    SC_HANDLE schSCManager = OpenSCManager(
        NULL,                   // 目标计算机的名称,NULL：连接本地计算机上的服务控制管理器
        NULL,                   // 服务控制管理器数据库的名称，NULL：打开 SERVICES_ACTIVE_DATABASE 数据库
        SC_MANAGER_ALL_ACCESS   // 所有权限
    );
    if (schSCManager == NULL) {
        CloseServiceHandle(schSCManager);
        return FALSE;
    }

    // 创建服务对象，添加至服务控制管理器数据库
    SC_HANDLE schService = CreateService(
        schSCManager,               // 服务控件管理器数据库的句柄
        serviceName,                // 要安装的服务的名称
        serviceName,                // 用户界面程序用来标识服务的显示名称
        SERVICE_ALL_ACCESS,         // 对服务的访问权限：所有全权限
        SERVICE_KERNEL_DRIVER,      // 服务类型：驱动服务
        SERVICE_DEMAND_START,       // 服务启动选项：进程调用 StartService 时启动
        SERVICE_ERROR_IGNORE,       // 如果无法启动：忽略错误继续运行
        drvPath,                    // 驱动文件绝对路径，如果包含空格需要多加双引号
        NULL,                       // 服务所属的负载订购组：服务不属于某个组
        NULL,                       // 接收订购组唯一标记值：不接收
        NULL,                       // 服务加载顺序数组：服务没有依赖项
        NULL,                       // 运行服务的账户名：使用 LocalSystem 账户
        NULL                        // LocalSystem 账户密码
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

// 启动服务
BOOL startDvr(CONST WCHAR serviceName[20]) {

    // 打开服务控制管理器数据库
    SC_HANDLE schSCManager = OpenSCManager(
        NULL,                   // 目标计算机的名称,NULL：连接本地计算机上的服务控制管理器
        NULL,                   // 服务控制管理器数据库的名称，NULL：打开 SERVICES_ACTIVE_DATABASE 数据库
        SC_MANAGER_ALL_ACCESS   // 所有权限
    );
    if (schSCManager == NULL) {
        CloseServiceHandle(schSCManager);
        return FALSE;
    }

    // 打开服务
    SC_HANDLE hs = OpenService(
        schSCManager,           // 服务控件管理器数据库的句柄
        serviceName,            // 要打开的服务名
        SERVICE_ALL_ACCESS      // 服务访问权限：所有权限
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

// 停止服务
BOOL stopDvr(CONST WCHAR serviceName[20]) {

    // 打开服务控制管理器数据库
    SC_HANDLE schSCManager = OpenSCManager(
        NULL,                   // 目标计算机的名称,NULL：连接本地计算机上的服务控制管理器
        NULL,                   // 服务控制管理器数据库的名称，NULL：打开 SERVICES_ACTIVE_DATABASE 数据库
        SC_MANAGER_ALL_ACCESS   // 所有权限
    );
    if (schSCManager == NULL) {
        CloseServiceHandle(schSCManager);
        return FALSE;
    }

    // 打开服务
    SC_HANDLE hs = OpenService(
        schSCManager,           // 服务控件管理器数据库的句柄
        serviceName,            // 要打开的服务名
        SERVICE_ALL_ACCESS      // 服务访问权限：所有权限
    );
    if (hs == NULL) {
        CloseServiceHandle(hs);
        CloseServiceHandle(schSCManager);
        return FALSE;
    }

    // 如果服务正在运行
    SERVICE_STATUS status;
    if (QueryServiceStatus(hs, &status) == 0) {
        CloseServiceHandle(hs);
        CloseServiceHandle(schSCManager);
        return FALSE;
    }
    if (status.dwCurrentState != SERVICE_STOPPED &&
        status.dwCurrentState != SERVICE_STOP_PENDING
        ) {
        // 发送关闭服务请求
        if (ControlService(
            hs,                         // 服务句柄
            SERVICE_CONTROL_STOP,       // 控制码：通知服务应该停止
            &status                     // 接收最新的服务状态信息
        ) == 0) {
            CloseServiceHandle(hs);
            CloseServiceHandle(schSCManager);
            return FALSE;
        }

        // 判断超时
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

// 卸载驱动
BOOL unloadDvr(CONST WCHAR serviceName[20]) {

    // 打开服务控制管理器数据库
    SC_HANDLE schSCManager = OpenSCManager(
        NULL,                   // 目标计算机的名称,NULL：连接本地计算机上的服务控制管理器
        NULL,                   // 服务控制管理器数据库的名称，NULL：打开 SERVICES_ACTIVE_DATABASE 数据库
        SC_MANAGER_ALL_ACCESS   // 所有权限
    );
    if (schSCManager == NULL) {
        CloseServiceHandle(schSCManager);
        return FALSE;
    }

    // 打开服务
    SC_HANDLE hs = OpenService(
        schSCManager,           // 服务控件管理器数据库的句柄
        serviceName,            // 要打开的服务名
        SERVICE_ALL_ACCESS      // 服务访问权限：所有权限
    );
    if (hs == NULL) {
        CloseServiceHandle(hs);
        CloseServiceHandle(schSCManager);
        return FALSE;
    }

    // 删除服务
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
    DWORD dwSize = 0;
    if (ModuleName != NULL)
        wcscpy_s(buf, ModuleName);
    ReadWriteBegin();
    BOOL ret = DeviceIoControl(Handle, ID_GetModel, &data, sizeof(data), &data, sizeof(data), &dwSize, NULL);
    ReadWriteEnd();
    return *(ULONG64*)&buf;
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
