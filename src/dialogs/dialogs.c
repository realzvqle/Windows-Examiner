#include "dialogs.h"
#include "power/power.h"
#include "run/run.h"
#include <stdint.h>



uint8_t currentdialog = 0;



void ChangeDialogState(uint8_t dialognum){
    currentdialog = dialognum;
}

uint8_t GetCurrentDialogState(){
    return currentdialog;
}

void DrawAllDialogs(){
    switch(currentdialog){
        case 0:
            break;
        case 1:
            DrawRunDialogBox();
            break;
        case 2:
            DrawPowerDialogBox();
            break;
        default:
            currentdialog = 0;
            break;   
    }
}