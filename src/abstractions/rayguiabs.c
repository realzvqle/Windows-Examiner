#define RAYGUI_IMPLEMENTATION
#include "../externheaders/raygui.h"
#include "../drawing/drawing.h"
#include "rayguiabs.h"



void RayGUIInitialize(){
    GuiSetFont(InsGetFont());
    GuiSetStyle(DEFAULT, TEXT_SIZE, 25); 
    GuiSetStyle(LISTVIEW, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT); 

}

int RayGUIDrawButton(int x, int y, int sizeX, int sizeY, const char* text){
    Rectangle rec = {x, y, sizeX, sizeY};
    return GuiButton(rec, text);
}

int RayGUIDrawTextBox(int x, int y, int sizeX, int sizeY, char* text, int textSize, BOOL editMode){
    Rectangle rec = {x, y, sizeX, sizeY};
    return GuiTextBox(rec, text, textSize, editMode);
}

int RayGUIDrawList(int x, int y, int sizeX, int sizeY, char *text, int *scrollIndex, int *active){
    Rectangle rec = {x, y, sizeX, sizeY};
    return GuiListView(rec, text, scrollIndex, active);
}

int RayGUIDrawDialog(int x, int y, int sizeX, int sizeY, const char* title){
    Rectangle rec = {x, y, sizeX, sizeY};
    return GuiWindowBox(rec, title);
}

int RayGUIDrawDialogRec(Rectangle bounds, const char* title){
    return GuiWindowBox(bounds, title);
}