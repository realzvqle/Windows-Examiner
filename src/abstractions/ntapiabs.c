#include "ntapiabs.h"
#include <libloaderapi.h>
#include <minwindef.h>
#include <winnt.h>
#include <stdio.h>
#include <winternl.h>


// would just simply define the functions and add ntdll.lib but for reason xmake doesnt like ntdll.lib so


typedef NTSTATUS(NTAPI *NtShutdownSystemPtr)(
    SHUTDOWN_ACTION
);

typedef NTSTATUS(NTAPI *RtlAdjustPrivilegePtr)(
    ULONG,
    BOOLEAN,
    BOOLEAN,
    PBOOLEAN
);

typedef NTSTATUS(NTAPI *NtRaiseHardErrorPtr)(
    NTSTATUS,
    ULONG,
    ULONG,
    PULONG_PTR,
    ULONG,
    PULONG
);

typedef NTSTATUS(NTAPI *NtOpenProcessPtr)(
    PHANDLE,
    ACCESS_MASK,
    PCOBJECT_ATTRIBUTES,
    CLIENT_ID*
);

typedef NTSTATUS(NTAPI *NtSuspendProcessPtr)(
    HANDLE
);

typedef NTSTATUS(NTAPI *NtResumeProcessPtr)(
    HANDLE
);


static HMODULE ntdll;

void NtAbstractionInitialize(){
    ntdll = GetModuleHandleW(L"ntdll.dll");
}

void NtAbstractionClose(){
    FreeLibrary(ntdll);
}

NTSTATUS NtShutdownSystem(SHUTDOWN_ACTION action){
    NtShutdownSystemPtr ntshutdownsystem = (NtShutdownSystemPtr)GetProcAddress(ntdll, "NtShutdownSystem");
    if(!ntshutdownsystem){
        wprintf(L"Couldn't find function from dll\n");
        return NULL;
    }
    NTSTATUS status = ntshutdownsystem(action);
    return status;
}   

NTSTATUS NtOpenProcess(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, PCOBJECT_ATTRIBUTES ObjectAttributes,
                         CLIENT_ID* ClientId)
{
    NtOpenProcessPtr ntopen = (NtOpenProcessPtr)GetProcAddress(ntdll, "NtOpenProcess");
    if(!ntopen){
        wprintf(L"Couldn't find function from dll\n");
        return NULL;
    }
    NTSTATUS status = ntopen(ProcessHandle, DesiredAccess, ObjectAttributes, ClientId);
    return status;
}   


NTSTATUS RtlAdjustPrivilege(ULONG Privlege, BOOLEAN Enable, BOOLEAN Client, PBOOLEAN wasEnabled){
    RtlAdjustPrivilegePtr rtladjustpriv = (RtlAdjustPrivilegePtr)GetProcAddress(ntdll, "RtlAdjustPrivilege");
    if(!rtladjustpriv){
        wprintf(L"Couldn't find function from dll\n");
        return NULL;
    }
    NTSTATUS status = rtladjustpriv(Privlege, Enable, Client, wasEnabled);
    return status;
}

NTSTATUS NtRaiseHardError(NTSTATUS ErrorStatus, ULONG NumberOfParameters,
                            ULONG UnicodeStringParameterMask,PULONG_PTR Parameters, 
                            ULONG ValidResponseOptions,PULONG Response)
{
    NtRaiseHardErrorPtr ntraisehrderr = (NtRaiseHardErrorPtr)GetProcAddress(ntdll, "NtRaiseHardError");
    if(!ntraisehrderr){
        wprintf(L"Couldn't find function from dll\n");
        return NULL;
    }
    NTSTATUS status = ntraisehrderr(ErrorStatus, NumberOfParameters, 
                                    UnicodeStringParameterMask, Parameters, ValidResponseOptions, Response);
    return status;
}

NTSTATUS NtSuspendProcess(HANDLE hProcess) {
    NtSuspendProcessPtr ntsuspendproces = (NtSuspendProcessPtr)GetProcAddress(ntdll, "NtSuspendProcess");
    if(!ntsuspendproces){
        wprintf(L"Couldn't find function from dll\n");
        return NULL;
    }
    NTSTATUS status = ntsuspendproces(hProcess);
    return status;
}

NTSTATUS NtResumeProcess(HANDLE hProcess) {
    NtResumeProcessPtr ntresumeproces = (NtResumeProcessPtr)GetProcAddress(ntdll, "NtResumeProcess");
    if(!ntresumeproces){
        wprintf(L"Couldn't find function from dll\n");
        return NULL;
    }
    NTSTATUS status = ntresumeproces(hProcess);
    return status;
}

void ShowNtStatusError(NTSTATUS status){
    ULONG response;
    NtRaiseHardError(status, 0, 0, 0, OptionOk, &response);
}

NTSTATUS SimpleAdjustPrivilege(ULONG Privlege, BOOLEAN Enable){
    BOOLEAN enabled;
    NTSTATUS result = RtlAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, Enable, FALSE, &enabled);
    if(!NT_SUCCESS(result)){
        ShowNtStatusError(result);
    }
    return result;
}

