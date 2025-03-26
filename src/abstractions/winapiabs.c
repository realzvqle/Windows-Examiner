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

void* AllocateMemory(size_t size) {
    return VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

BOOL DeallocateMemory(void* memory) {
    return VirtualFree(memory, 0, MEM_RELEASE);
}

char* WCharToChar(WCHAR* string){
    size_t len = wcstombs(NULL, string, 0) + 1;
    char* cstr = (char*)AllocateMemory(len);
    wcstombs(cstr, string, len);
    return cstr;
}

WCHAR* CharToWChar(char* string){
    size_t len = mbstowcs(NULL, string, 0) + 1;
    WCHAR* wstr = (WCHAR*)AllocateMemory(len);
    mbstowcs(wstr, string, len);
    return wstr;
}