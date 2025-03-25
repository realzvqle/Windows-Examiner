
#include "abstractions/ntapiabs.h"
#include "abstractions/rayguiabs.h"
#include "dialogs/dialogs.h"
#include "externheaders/raylib.h"
#include "drawing/drawing.h"
#include "processlist/processlist.h"
#include "topbar/menu/menu.h"
#include <lmcons.h>
#include "externheaders/raygui.h"
#include <winbase.h>
#include <stdio.h>
#include <winnt.h>
int main(){
    SimpleAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, TRUE);
    SimpleAdjustPrivilege(SE_DEBUG_PRIVILEGE, TRUE);

    char computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD csize = sizeof(computerName);
    GetComputerNameA(computerName, &csize);

    char username[UNLEN + 1];
    DWORD usize = sizeof(username);
    GetUserNameA(username, &usize);

    char title[1024];
    sprintf(title, "Windows Examiner - [%s/%s]", computerName, username);


    SetTraceLogLevel(LOG_ERROR);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(920, 850, title);
    Image image = LoadImage("resources/icon.png");
    SetWindowIcon(image);
    SetupDrawings();
    RayGUIInitialize();

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);
        if(GetCurrentDialogState() == 0){
            RenderTopBarMenu();
            RenderProcessList();
        }
        DrawAllDialogs();
        EndDrawing();
    }
    UnloadImage(image);
    CloseDrawings();
    CloseWindow();
}