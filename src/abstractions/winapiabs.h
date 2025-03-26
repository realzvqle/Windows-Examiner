#ifndef WINAPIABS_H_INCLUDED
#define WINAPIABS_H_INCLUDED





#include "../superheader.h"


int SimpleMessageBox(LPCWSTR text, UINT uType);
void ShowFailureResponse(DWORD errorCode);

void* AllocateMemory(size_t size);
BOOL DeallocateMemory(void* memory);
char* WCharToChar(WCHAR* string);
WCHAR* CharToWChar(char* string);
#endif