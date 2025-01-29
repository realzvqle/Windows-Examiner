
#include "abstractions/rayguiabs.h"
#include "abstractions/winapiabs.h"
#include "dialogs/dialogs.h"
#include "externheaders/raylib.h"
#include "drawing/drawing.h"
#include "topbar/menu/menu.h"

#include "externheaders/raygui.h"
int main(){
    GivePowerPermissions();
    SetTraceLogLevel(LOG_ERROR);
    
    InitWindow(920, 850, "Windows Examiner");
    Image image = LoadImage("resources/icon.png");
    SetWindowIcon(image);
    SetupDrawings();
    RayGUIInitialize();
    GuiSetStyle(DEFAULT, TEXT_SIZE, 25); 

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);
        if(GetCurrentDialogState() == 0){
            RenderTopBarMenu();
        }
        DrawAllDialogs();
        
        EndDrawing();
    }
    UnloadImage(image);
    CloseDrawings();
    CloseWindow();
}