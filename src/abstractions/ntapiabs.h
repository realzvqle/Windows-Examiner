#ifndef NTAPIABS_H_INCLUDED
#define NTAPIABS_H_INCLUDED







#include "../superheader.h"
#include <winternl.h>


typedef enum _SHUTDOWN_ACTION
{
    ShutdownNoReboot,
    ShutdownReboot,
    ShutdownPowerOff,
    ShutdownRebootForRecovery 
} SHUTDOWN_ACTION;

NTSTATUS NtShutdownSystem(SHUTDOWN_ACTION action);

#endif