#include "winapiabs.h"




int SimpleMessageBox(LPCWSTR text, UINT uType){
    return MessageBoxW(NULL, text, L"Windows Examiner", uType);
}


void ShowFailureResponse(DWORD errorCode){
    LPVOID lpMsgBuf;
    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&lpMsgBuf,
        0, NULL);

    SimpleMessageBox((LPWSTR)lpMsgBuf, MB_OK | MB_ICONERROR);  
    LocalFree(lpMsgBuf);
}

BOOL GivePowerPermissions(){
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)){
        ShowFailureResponse(GetLastError());
        return FALSE;
    }
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, (PLUID)&tkp.Privileges);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if(!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0)){
        ShowFailureResponse(GetLastError());
        CloseHandle(hToken);
        return FALSE;
    }
    CloseHandle(hToken);
    return TRUE;
}