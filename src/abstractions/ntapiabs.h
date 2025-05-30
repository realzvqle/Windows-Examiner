#ifndef NTAPIABS_H_INCLUDED
#define NTAPIABS_H_INCLUDED







#include "../superheader.h"
#include <winternl.h>

#define SE_SHUTDOWN_PRIVILEGE (19L)
#define SE_DEBUG_PRIVILEGE (20L)

typedef const OBJECT_ATTRIBUTES *PCOBJECT_ATTRIBUTES;
typedef enum _SHUTDOWN_ACTION
{
    ShutdownNoReboot,
    ShutdownReboot,
    ShutdownPowerOff,
    ShutdownRebootForRecovery 
} SHUTDOWN_ACTION;

typedef enum _HARDERROR_RESPONSE_OPTION
{
    OptionAbortRetryIgnore,
    OptionOk,
    OptionOkCancel,
    OptionRetryCancel,
    OptionYesNo,
    OptionYesNoCancel,
    OptionShutdownSystem,
    OptionOkNoWait,
    OptionCancelTryContinue
} HARDERROR_RESPONSE_OPTION;

void NtAbstractionInitialize();
void NtAbstractionClose();
NTSTATUS NtShutdownSystem(SHUTDOWN_ACTION action);
NTSTATUS RtlAdjustPrivilege(ULONG Privlege, BOOLEAN Enable, BOOLEAN Client, PBOOLEAN wasEnabled);
NTSTATUS NtRaiseHardError(NTSTATUS ErrorStatus, ULONG NumberOfParameters,
    ULONG UnicodeStringParameterMask,PULONG_PTR Parameters, 
    ULONG ValidResponseOptions,PULONG Response);
NTSTATUS NtOpenProcess(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, PCOBJECT_ATTRIBUTES ObjectAttributes,
    CLIENT_ID* ClientId);
NTSTATUS NtSuspendProcess(HANDLE hProcess);
NTSTATUS NtResumeProcess(HANDLE hProcess);
void ShowNtStatusError(NTSTATUS status);
NTSTATUS SimpleAdjustPrivilege(ULONG Privlege, BOOLEAN Enable);

#endif