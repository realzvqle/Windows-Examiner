#include "misc.h"
#include "../dialogs.h"
#include "../../abstractions/rayguiabs.h"
#include "../../abstractions/winapiabs.h"
#include <winuser.h>
#include <powrprof.h>
#include "../../abstractions/ntapiabs.h"


void SummonMiscDialogBox(){
    ChangeDialogState(3);
}

void DrawMiscDialogBox(){
    Rectangle dialogBox = { 30, 40, 500, 220 };
    Rectangle winBox = { 0, 0, 600, 320 };
    Rectangle textBox = { 10, 100, 400, 40};
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
    int winstate = RayGUIDrawDialogRec(winBox, "Power");
    if(winstate == 1) ChangeDialogState(0);
    int aboutButton = RayGUIDrawButton(dialogBox.x + 5, dialogBox.y + 10, 120, 40, "About");
    int exitbutton = RayGUIDrawButton(dialogBox.x + 5, dialogBox.y + 170, 90, 40, "Exit");

    if(aboutButton == 1) {
        WCHAR buffer[1024];
        swprintf_s(buffer, 1024, L"\tWindows-Examiner Build %s\t\n\t%s\t\n\t%s\t\n", 
                                                             VERSIONW, L"Made by zvqle/zvqlifed/realzvqle (same person)",
                                                                       L"Contributions: None except zvqle =(");
        SimpleMessageBox(buffer, MB_OK);
    }
    if(exitbutton == 1){
        ChangeDialogState(0);
    }
}