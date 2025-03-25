#include "processlist.h"
#include <Psapi.h>
#include <errhandlingapi.h>
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

#define MAX_PROCESSES 4096
#define MAX_ENTRY_SIZE 256
static CHAR newarr[MAX_PROCESSES][MAX_ENTRY_SIZE];
static const char* processlistrender[MAX_PROCESSES];
static int active;
static int scrollindex;
static int focus;
static bool initprocesslister = false;
static int index = 0;

static void PrintProcessInfo(DWORD pid){
    if (index >= MAX_PROCESSES) return;
    CHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
    HANDLE hProcess = NULL;
    OBJECT_ATTRIBUTES obj = {0};
    CLIENT_ID client = {0};
    client.UniqueProcess = (HANDLE)pid;
    client.UniqueThread = 0;
    NTSTATUS status = NtOpenProcess(&hProcess, MAXIMUM_ALLOWED, &obj, &client);
    if(!NT_SUCCESS(status)){
        //ShowNtStatusError(status);
        return;
    }
    
    HMODULE hMod;
    DWORD cbNeeded;
    if(EnumProcessModulesEx(hProcess, &hMod, sizeof(hMod), &cbNeeded, LIST_MODULES_ALL)){
        GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
    } else return;
    
    sprintf(newarr[index], TEXT("%s-%lu"), szProcessName, pid);
    index++;
    CloseHandle(hProcess);
}


static inline void UpdateProcessList(){
    index = 0;
    DWORD aProcesses[MAX_PROCESSES], cbNeeded, cProcesses;
    if(!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)){
        ShowFailureResponse(GetLastError());
        return;
    }
    
    cProcesses = cbNeeded / sizeof(DWORD);

    for(int i = 0; i < cProcesses; i++){
        if(aProcesses[i] != 0){
            PrintProcessInfo(aProcesses[i]);
        }
    }
}

static inline void KillProcess(int index){
    int count = 0;
    const char** item = RayGUITextSplit(processlistrender[active], '-', &count, NULL);

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


void RenderProcessList(){
    static bool init = false;
    static int prevtime = 0;
    if(init == false){
        UpdateProcessList();
        prevtime = GetTime();
        init = true;
        initprocesslister = false;
    }
    if((GetTime() - prevtime) >= 1){
        initprocesslister = false;
        UpdateProcessList();
        prevtime = GetTime();
        // very unoptimized, need to be fixed later
        for (int i = 0; i < MAX_PROCESSES && i < index; i++) {
            processlistrender[i] = newarr[i];  
        }
        printf("Updated!\n");
    }
    int r = RayGUIDrawListEx(0, 35, (float)GetScreenWidth(), (float)GetScreenHeight() - 35, 
    (const char **)processlistrender, index,&scrollindex, &focus, &active);
    if(IsKeyPressed('W')){
        if(processlistrender[active] == NULL) return;
        int result = SimpleMessageBox(L"Are you sure you want to kill this process?", 
            MB_YESNO | MB_ICONQUESTION);
        if(result == IDNO) return;
        KillProcess(active);
    }
    //printf("\n\n%s\n\n", array);
}


