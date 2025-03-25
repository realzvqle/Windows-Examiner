
#include "abstractions/rayguiabs.h"
#include "abstractions/winapiabs.h"
#include "dialogs/dialogs.h"
#include "externheaders/raylib.h"
#include "drawing/drawing.h"
#include "processlist/processlist.h"
#include "topbar/menu/menu.h"

#include "externheaders/raygui.h"
int main(){
    GivePowerPermissions();
    GiveProcessPermissions();
    SetTraceLogLevel(LOG_ERROR);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(920, 850, "Windows Examiner");
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