#include "menu.h"
#include "../../drawing/drawing.h"
#include "../../abstractions/winapiabs.h"
#include "../../abstractions/rayguiabs.h"
#include "../../dialogs/run/run.h"
#include "../../dialogs/power/power.h"
#include <synchapi.h>
#include <winbase.h>
#include <winuser.h>




void RenderTopBarMenu(){
    DrawRectangle(0, 0, GetScreenWidth(), 35, LIGHTGRAY);
    int runbuttonclicked = RayGUIDrawButton(0, 0, 70, 35, "Run");
    if(runbuttonclicked == 1){
        SummonRunDialogBox();
    }
    int powerButtonClicked = RayGUIDrawButton(70, 0, 70, 35, "Power");
    if(powerButtonClicked == 1){
        SummonPowerDialogBox();
    }
    DrawRectangle(0, 35, GetScreenWidth(), 2, DARKGRAY);
}