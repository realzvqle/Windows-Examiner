#include "run.h"
#include "../../drawing/drawing.h"
#include "../dialogs.h"
#include "../../abstractions/winapiabs.h"
#include "../../abstractions/rayguiabs.h"
#include <stdio.h>
#include <errhandlingapi.h>
#include <minwindef.h>
#include <wchar.h>
#include <winuser.h>


#include <shellapi.h>

void SummonRunDialogBox(){
    ChangeDialogState(1);
}


void DrawRunDialogBox() {
    static BOOL adminRun = FALSE;
    static BOOL trustedInstaRun = FALSE;

    Rectangle dialogBox = { 0, 0, 500, 250 };
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);

    int winstate = RayGUIDrawDialogRec(dialogBox, "Run");
    if(winstate == 1) ChangeDialogState(0);

    static char buffer[512] = { 0 };
    RayGUIDrawTextBox(10, 100, 400, 40, buffer, 100, true);
    RayGUIDrawText("Enter the Application to Run:", dialogBox.x + 20, dialogBox.y + 30, 25);

    int adminCheckBox = RayGUIDrawButton(dialogBox.x + 10, dialogBox.y + 152, 15, 15, "");
    if(adminCheckBox == 1){
        if(adminRun == TRUE){
            adminRun = FALSE;
        } 
        else{
            adminRun = TRUE;
        } 
    } 
    char admintext[512];
    if(adminRun == TRUE) sprintf(admintext, "Run As Administrator [Selected]");
    else sprintf(admintext, "Run As Administrator");
    RayGUIDrawText(admintext, dialogBox.x + 40, dialogBox.y + 150, 20);

    int trustedinstaCheckBox = RayGUIDrawButton(dialogBox.x + 10, dialogBox.y + 170, 15, 15, "");
    if(trustedinstaCheckBox == 1){
        // if(trustedInstaRun == TRUE) trustedInstaRun = FALSE;
        // else{
        //     trustedInstaRun = TRUE;
        //     adminRun = TRUE;
        // } 
        SimpleMessageBox(L"Not Implemented", MB_OK | MB_ICONINFORMATION);
    }
    char trustedinstatext[512];
    if(trustedInstaRun == TRUE) sprintf(trustedinstatext, "Run As TrustedInstaller [Selected]");
    else sprintf(trustedinstatext, "Run As TrustedInstaller");
    RayGUIDrawText(trustedinstatext, dialogBox.x + 40, dialogBox.y + 168, 20);


    int runNormalButton = RayGUIDrawButton(dialogBox.x + 10, dialogBox.y + 200, 70, 35, "Run");
    if(runNormalButton == 1){
        WCHAR lpOperation[90];
        wcscpy(lpOperation, adminRun ? L"runas" : L"open");
        WCHAR buf[512];
        mbstowcs(buf, buffer, 512);
        WCHAR* context;
        WCHAR* command = wcstok(buf, L" ", &context);
        WCHAR* arg = wcstok(NULL, L"\0", &context);
        HINSTANCE result = ShellExecuteW(NULL, lpOperation, command, arg, NULL, SW_SHOW);
        if(result <= (HINSTANCE)32){
            ShowFailureResponse(GetLastError());
        }
        for(int i = 0; i < 511; i++){
            if(buffer[i] == 0){
                break;
            }
            buffer[i] = 0;
        } 
        adminRun = FALSE;
        trustedInstaRun = FALSE;
        ChangeDialogState(0);
    }
}



