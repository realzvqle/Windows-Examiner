#include "processlist.h"
#include <Psapi.h>
#include <errhandlingapi.h>
#include <handleapi.h>
#include <minwindef.h>
#include <string.h>
#include <winbase.h>
#include <winnt.h>
#include "../abstractions/winapiabs.h"
#include <tchar.h>
#include <stdio.h>
#include <winternl.h>
#include <winuser.h>
#include "../abstractions/rayguiabs.h"
#include "../abstractions/ntapiabs.h"
#include <tlhelp32.h>

struct ProcessInfo {
    DWORD pid;
    DWORD threads;
    DWORD parentpid;
};

#define MAX_PROCESSES 4096
static struct ProcessInfo pinfo[MAX_PROCESSES];
static const char* processlist[MAX_PROCESSES];
static int active;
static int scrollindex;
static int focus;
static int index = 0;

static void PrintProcessInfo(){
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32W entry = {0};
    entry.dwSize = sizeof(PROCESSENTRY32W);
    if(Process32FirstW(hSnapshot, &entry)){
        do {
            char* namecstr = WCharToChar(entry.szExeFile);
            char temp[1024];
            sprintf(temp, TEXT("%s"), namecstr);
            processlist[index] = _strdup(temp);
            pinfo[index].pid = entry.th32ProcessID;
            pinfo[index].parentpid = entry.th32ParentProcessID;
            pinfo[index].threads = entry.cntThreads;
            index++;
            DeallocateMemory(namecstr);
        } while(Process32NextW(hSnapshot, &entry));
    }
    CloseHandle(hSnapshot);
}


static inline void UpdateProcessList(){
    index = 0;
    PrintProcessInfo();
}

static inline void KillProcess(){
    int result = SimpleMessageBox(L"Are you sure you want to kill this process?", 
        MB_YESNO | MB_ICONQUESTION);
    if(result == IDNO) return;

    HANDLE hProcess = NULL;
    OBJECT_ATTRIBUTES obj = {0};
    CLIENT_ID client = {0};
    client.UniqueProcess = (HANDLE)pinfo[active].pid;
    client.UniqueThread = 0;
    NTSTATUS status = NtOpenProcess(&hProcess, MAXIMUM_ALLOWED, &obj, &client);
    if(!NT_SUCCESS(status)){
        ShowNtStatusError(status);
        return;
    }
    TerminateProcess(hProcess, 0);
    CloseHandle(hProcess);
}

static inline void SuspendProcess(){
    HANDLE hProcess = NULL;
    OBJECT_ATTRIBUTES obj = {0};
    CLIENT_ID client = {0};
    client.UniqueProcess = (HANDLE)pinfo[active].pid;
    client.UniqueThread = 0;
    NTSTATUS status = NtOpenProcess(&hProcess, MAXIMUM_ALLOWED, &obj, &client);
    if(!NT_SUCCESS(status)){
        ShowNtStatusError(status);
        return;
    }
    NtSuspendProcess(hProcess);
    // char temp[1024];
    // sprintf(temp, "(Suspended)%s", processlist[active]);
    // processlist[index] = _strdup(temp);
    CloseHandle(hProcess);
}

static inline void ResumeProcess(){

    HANDLE hProcess = NULL;
    OBJECT_ATTRIBUTES obj = {0};
    CLIENT_ID client = {0};
    client.UniqueProcess = (HANDLE)pinfo[active].pid;
    client.UniqueThread = 0;
    NTSTATUS status = NtOpenProcess(&hProcess, MAXIMUM_ALLOWED, &obj, &client);
    if(!NT_SUCCESS(status)){
        ShowNtStatusError(status);
        return;
    }
    NtResumeProcess(hProcess);
    // char temp[1024];
    // sprintf(temp, "(Suspended)%s", processlist[active]);
    // processlist[index] = _strdup(temp);
    CloseHandle(hProcess);
}

static inline void ProcessKeyBinds(){
    if(IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)){
        if(processlist[active] == NULL) return;
        switch(GetKeyPressed()){
            case KEY_Q:
                SuspendProcess();
                break;
            case KEY_W:
                KillProcess();
                break;
            case KEY_E:
                ResumeProcess();
                break;
        }
    }
}

static inline void ShowProcessInfo(){
    const int infodiasizey = 202; 
    RayGUIDrawDialog(GetMouseX(), GetMouseY(), 500, infodiasizey, processlist[focus]);
    char tempname[1024];
    sprintf(tempname, "Process Name: %s", processlist[focus]);
    RayGUIDrawText(tempname, GetMouseX() + 0, GetMouseY() + 22, 20);
    char temppid[1024];
    sprintf(temppid, "Process ID: %lu", pinfo[focus].pid);
    RayGUIDrawText(temppid, GetMouseX() + 0, GetMouseY() + 42, 20);
    char tempcntthread[1024];
    sprintf(tempcntthread, "Amount of Threads: %lu", pinfo[focus].threads);
    RayGUIDrawText(tempcntthread, GetMouseX() + 0, GetMouseY() + 62, 20);
    char tempcntusage[1024];
    sprintf(tempcntusage, "Parent Process PID: %lu", pinfo[focus].parentpid);
    RayGUIDrawText(tempcntusage, GetMouseX() + 0, GetMouseY() + 82, 20);

    RayGUIDrawText("Click on it and Press Shift+W to Terminate This Process", GetMouseX() + 0, 
                        GetMouseY() + (infodiasizey - 62), 20);
    RayGUIDrawText("Click on it and Press Shift+Q to Suspend This Process", GetMouseX() + 0, 
                        GetMouseY() + (infodiasizey - 42), 20);
    RayGUIDrawText("Click on it and Press Shift+E to Resume This Process", GetMouseX() + 0, 
                        GetMouseY() + (infodiasizey - 22), 20);
}


void RenderProcessList(){
    static bool init = false;
    static int prevtime = 0;
    if(init == false){
        UpdateProcessList();
        prevtime = GetTime();
        init = true;
    }
    if((GetTime() - prevtime) >= 1){
        UpdateProcessList();
        prevtime = GetTime();
        printf("Updated!\n");
    }
    int r = RayGUIDrawListEx(0, 35, (float)GetScreenWidth(), (float)GetScreenHeight() - 35, 
    (const char **)processlist, index,&scrollindex, &focus, &active);

    
    ProcessKeyBinds();
    ShowProcessInfo();
}


