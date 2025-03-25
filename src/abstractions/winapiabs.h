#ifndef WINAPIABS_H_INCLUDED
#define WINAPIABS_H_INCLUDED





#include "../superheader.h"


int SimpleMessageBox(LPCWSTR text, UINT uType);
void ShowFailureResponse(DWORD errorCode);
BOOL GivePowerPermissions();
BOOL GiveProcessPermissions();
#endif