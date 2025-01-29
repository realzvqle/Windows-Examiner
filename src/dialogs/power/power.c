#include "power.h"
#include "../dialogs.h"
#include "../../abstractions/rayguiabs.h"
#include "../../abstractions/winapiabs.h"
#include <winuser.h>
#include <powrprof.h>
#include "../../abstractions/ntapiabs.h"


void SummonPowerDialogBox(){
    ChangeDialogState(2);
}

static inline BOOL CheckIfShouldRun(){
    if(SimpleMessageBox(L"Are You Sure?\nMake Sure to Save Your Work!", MB_YESNO | MB_ICONQUESTION) == IDYES) return TRUE;
    else return FALSE;
}

void DrawPowerDialogBox(){
    Rectangle dialogBox = { 30, 40, 500, 220 };
    Rectangle textBox = { 10, 100, 400, 40};
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
    DrawRectangleRec(dialogBox, RAYWHITE);
    DrawRectangleLinesEx(dialogBox, 3, BLACK);

    int shutdownNormal = RayGUIDrawButton(dialogBox.x + 5, dialogBox.y + 10, 120, 40, "Shutdown");
    int restartNormal = RayGUIDrawButton(dialogBox.x + 126, dialogBox.y + 10, 120, 40, "Restart");
    int hibernate = RayGUIDrawButton(dialogBox.x + 247, dialogBox.y + 10, 120, 40, "Hibernate");
    int suspend = RayGUIDrawButton(dialogBox.x + 368, dialogBox.y + 10, 120, 40, "Suspend");

    int shutdownForce = RayGUIDrawButton(dialogBox.x + 5, dialogBox.y + 60, 300, 40, "Shutdown (Force)");
    int restartForce = RayGUIDrawButton(dialogBox.x + 5, dialogBox.y + 110, 300, 40, "Restart (Force)");

    
    int exitbutton = RayGUIDrawButton(dialogBox.x + 5, dialogBox.y + 170, 90, 40, "Exit");

    if(shutdownNormal == 1) {
        if(CheckIfShouldRun() == FALSE){
            ChangeDialogState(0);
            return;
        } 
        if (!InitiateSystemShutdownExW(NULL, L"System is Shutting Down NOW", 5, FALSE, FALSE, SHTDN_REASON_MAJOR_OTHER)) {
            ShowFailureResponse(GetLastError());
        }
        ChangeDialogState(0);
    }

    if(restartNormal == 1) {
        if(CheckIfShouldRun() == FALSE){
            ChangeDialogState(0);
            return;
        } 
        if (!InitiateSystemShutdownExW(NULL, L"System is Restarting NOW", 5, FALSE, TRUE, SHTDN_REASON_MAJOR_OTHER)) {
            ShowFailureResponse(GetLastError());
        }
        ChangeDialogState(0);
    }

    if(hibernate == 1){
        if(!SetSuspendState(TRUE, TRUE, FALSE)){
            ShowFailureResponse(GetLastError());
        }
        ChangeDialogState(0);
    }
    if(suspend == 1){
        if(!SetSuspendState(FALSE, TRUE, FALSE)){
            ShowFailureResponse(GetLastError());
        }
        ChangeDialogState(0);
    }

    if(shutdownForce == 1){
        if(CheckIfShouldRun() == FALSE){
            ChangeDialogState(0);
            return;
        } 
        NtShutdownSystem(ShutdownPowerOff);
        ChangeDialogState(0);
    }
    if(restartForce == 1){
        if(CheckIfShouldRun() == FALSE){
            ChangeDialogState(0);
            return;
        } 
        NtShutdownSystem(ShutdownReboot);
        ChangeDialogState(0);
    }
    if(exitbutton == 1){
        ChangeDialogState(0);
    }
}