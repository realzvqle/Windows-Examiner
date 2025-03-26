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

#define MAX_PROCESSES 4096
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
            sprintf(temp, TEXT("%s-%lu"), namecstr, entry.th32ProcessID);
            processlist[index] = _strdup(temp);
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
    int count = 0;
    const char** item = RayGUITextSplit(processlist[active], '-', &count, NULL);
    int pid = atoi(item[1]);
    if(!pid) return;

    HANDLE hProcess = NULL;
    OBJECT_ATTRIBUTES obj = {0};
    CLIENT_ID client = {0};
    client.UniqueProcess = (HANDLE)pid;
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
    int count = 0;
    const char** item = RayGUITextSplit(processlist[active], '-', &count, NULL);
    int pid = atoi(item[1]);
    if(!pid) return;

    HANDLE hProcess = NULL;
    OBJECT_ATTRIBUTES obj = {0};
    CLIENT_ID client = {0};
    client.UniqueProcess = (HANDLE)pid;
    client.UniqueThread = 0;
    NTSTATUS status = NtOpenProcess(&hProcess, MAXIMUM_ALLOWED, &obj, &client);
    if(!NT_SUCCESS(status)){
        ShowNtStatusError(status);
        return;
    }
    NtSuspendProcess(hProcess);
    char temp[1024];
    sprintf(temp, "(Suspended)%s", processlist[active]);
    processlist[index] = _strdup(temp);
    CloseHandle(hProcess);
}

static inline void ResumeProcess(){
    int count = 0;
    const char** item = RayGUITextSplit(processlist[active], '-', &count, NULL);
    int pid = atoi(item[1]);
    if(!pid) return;

    HANDLE hProcess = NULL;
    OBJECT_ATTRIBUTES obj = {0};
    CLIENT_ID client = {0};
    client.UniqueProcess = (HANDLE)pid;
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
    if(IsKeyPressed('W')){
        if(processlist[active] == NULL) return;
        int result = SimpleMessageBox(L"Are you sure you want to kill this process?", 
            MB_YESNO | MB_ICONQUESTION);
        if(result == IDNO) return;
        KillProcess();
    }
    if(IsKeyPressed('Q')){
        if(processlist[active] == NULL) return;
        SuspendProcess();
    }
    if(IsKeyPressed('E')){
        if(processlist[active] == NULL) return;
        ResumeProcess();
    }
}


