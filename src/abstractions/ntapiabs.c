#include "ntapiabs.h"
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





NTSTATUS NtShutdownSystem(SHUTDOWN_ACTION action){
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    if(!ntdll){
        printf("Couldn't get DLL\n");
        return STATUS_DLL_NOT_FOUND;
    }
    NtShutdownSystemPtr ntshutdownsystem = (NtShutdownSystemPtr)GetProcAddress(ntdll, "NtShutdownSystem");
    if(!ntshutdownsystem){
        printf("Couldn't find function from dll\n");
        return NULL;
    }
    NTSTATUS status = ntshutdownsystem(action);
    FreeLibrary(ntdll);
    return status;
}   

NTSTATUS NtOpenProcess(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, PCOBJECT_ATTRIBUTES ObjectAttributes,
                         CLIENT_ID* ClientId)
{
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    if(!ntdll){
        printf("Couldn't get DLL\n");
        return STATUS_DLL_NOT_FOUND;
    }
    NtOpenProcessPtr ntopen = (NtOpenProcessPtr)GetProcAddress(ntdll, "NtOpenProcess");
    if(!ntopen){
        printf("Couldn't find function from dll\n");
        return NULL;
    }
    NTSTATUS status = ntopen(ProcessHandle, DesiredAccess, ObjectAttributes, ClientId);
    FreeLibrary(ntdll);
    return status;
}   


NTSTATUS RtlAdjustPrivilege(ULONG Privlege, BOOLEAN Enable, BOOLEAN Client, PBOOLEAN wasEnabled){
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    if(!ntdll){
        printf("Couldn't get DLL\n");
        return STATUS_DLL_NOT_FOUND;
    }
    RtlAdjustPrivilegePtr rtladjustpriv = (RtlAdjustPrivilegePtr)GetProcAddress(ntdll, "RtlAdjustPrivilege");
    if(!rtladjustpriv){
        printf("Couldn't find function from dll\n");
        return NULL;
    }
    NTSTATUS status = rtladjustpriv(Privlege, Enable, Client, wasEnabled);
    FreeLibrary(ntdll);
    return status;
}

NTSTATUS NtRaiseHardError(NTSTATUS ErrorStatus, ULONG NumberOfParameters,
                            ULONG UnicodeStringParameterMask,PULONG_PTR Parameters, 
                            ULONG ValidResponseOptions,PULONG Response)
{
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    if(!ntdll){
        printf("Couldn't get DLL\n");
        return STATUS_DLL_NOT_FOUND;
    }
    NtRaiseHardErrorPtr ntraisehrderr = (NtRaiseHardErrorPtr)GetProcAddress(ntdll, "NtRaiseHardError");
    if(!ntraisehrderr){
        printf("Couldn't find function from dll\n");
        return NULL;
    }
    NTSTATUS status = ntraisehrderr(ErrorStatus, NumberOfParameters, 
                                    UnicodeStringParameterMask, Parameters, ValidResponseOptions, Response);
    FreeLibrary(ntdll);
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

