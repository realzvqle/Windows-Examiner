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