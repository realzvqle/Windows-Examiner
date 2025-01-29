#define RAYGUI_IMPLEMENTATION
#include "../externheaders/raygui.h"
#include "../drawing/drawing.h"
#include "rayguiabs.h"



void RayGUIInitialize(){
    GuiSetFont(InsGetFont());
}

int RayGUIDrawButton(int x, int y, int sizeX, int sizeY, const char* text){
    Rectangle rec = {x, y, sizeX, sizeY};
    return GuiButton(rec, text);
}

int RayGUIDrawTextBox(int x, int y, int sizeX, int sizeY, char* text, int textSize, BOOL editMode){
    Rectangle rec = {x, y, sizeX, sizeY};
    return GuiTextBox(rec, text, textSize, editMode);
}