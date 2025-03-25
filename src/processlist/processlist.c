#include "processlist.h"
#include <Psapi.h>
#include <errhandlingapi.h>
#include <minwindef.h>
#include <winbase.h>
#include <winnt.h>
#include "../abstractions/winapiabs.h"
#include <tchar.h>
#include <stdio.h>
#include "../abstractions/rayguiabs.h"

static CHAR array[1388400];
static int active;
static int scrollindex;
static bool initprocesslister = false;

static void PrintProcessInfo(DWORD pid){
    CHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if(hProcess == NULL){
        //ShowFailureResponse(GetLastError());
        return;
    }
    HMODULE hMod;
    DWORD cbNeeded;
    if(EnumProcessModulesEx(hProcess, &hMod, sizeof(hMod), &cbNeeded, LIST_MODULES_ALL)){
        GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
    } else return;
    if(initprocesslister == false){
        initprocesslister = true;
        sprintf(array, TEXT(""));
        sprintf(array, TEXT("%s - %lu;"), szProcessName, pid);

    }
    else {
        sprintf(array, TEXT("%s%s - %lu;"), array, szProcessName, pid);
    }
    CloseHandle(hProcess);
}


static inline void UpdateProcessList(){
    DWORD aProcesses[1024], cbNeeded, cProcesses;
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
        printf("Updated!\n");
    }
    RayGUIDrawList(0, 35, (float)GetScreenWidth(), (float)GetScreenHeight() - 35, 
                    array, &scrollindex, &active);
}