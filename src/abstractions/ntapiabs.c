#include "ntapiabs.h"






typedef NTSTATUS(NTAPI *NtShutdownSystemPtr)(
    SHUTDOWN_ACTION
);

NTSTATUS NtShutdownSystem(SHUTDOWN_ACTION action){
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    if(!ntdll){
        return STATUS_DLL_NOT_FOUND;
    }
    NtShutdownSystemPtr ntshutdownsystem = (NtShutdownSystemPtr)GetProcAddress(ntdll, "NtShutdownSystem");
    if(!ntshutdownsystem){
        return NULL;
    }
    NTSTATUS status = ntshutdownsystem(action);
    FreeLibrary(ntdll);
    return status;
}   