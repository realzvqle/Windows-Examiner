#include "menu.h"
#include "../../abstractions/rayguiabs.h"
#include "../../dialogs/run/run.h"
#include "../../dialogs/power/power.h"
#include "../../dialogs/misc/misc.h"
#include <synchapi.h>
#include <winbase.h>
#include <winuser.h>




void RenderTopBarMenu(){
    RayGUIDrawButton(0, 0, GetScreenWidth(), 35, "");
    int runbuttonclicked = RayGUIDrawButton(0, 0, 70, 35, "Run");
    if(runbuttonclicked == 1){
        SummonRunDialogBox();
    }
    int powerButtonClicked = RayGUIDrawButton(70, 0, 70, 35, "Power");
    if(powerButtonClicked == 1){
        SummonPowerDialogBox();
    }
    int miscButtonClicked = RayGUIDrawButton(GetScreenWidth() - 20, 0, 20, 35, "?");
    if(miscButtonClicked == 1){
        SummonMiscDialogBox();
    }
    DrawRectangle(0, 35, GetScreenWidth(), 2, DARKGRAY);
}