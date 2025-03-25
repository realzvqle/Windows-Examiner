#define RAYGUI_IMPLEMENTATION
#include "../externheaders/raygui.h"
#include "../drawing/drawing.h"
#include "rayguiabs.h"



void RayGUIInitialize(){
    GuiSetFont(InsGetFont());
    GuiLoadStyle("resources/style/style.rgs");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 15); 
    GuiSetStyle(LISTVIEW, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT); 
}

int RayGUIDrawButton(int x, int y, int sizeX, int sizeY, const char* text){
    Rectangle rec = {x, y, sizeX, sizeY};
    return GuiButton(rec, text);
}


void RayGUIDrawText(const char* text, int x, int y, int fontSize){
    Rectangle rec = {x, y, MeasureTextEx(GuiGetFont(), text, fontSize, 1).x,  MeasureTextEx(GuiGetFont(), text, fontSize, 1).y};
    GuiDrawText(text, rec, 0, WHITE);
}

int RayGUIDrawTextBox(int x, int y, int sizeX, int sizeY, char* text, int textSize, BOOL editMode){
    Rectangle rec = {x, y, sizeX, sizeY};
    return GuiTextBox(rec, text, textSize, editMode);
}

int RayGUIDrawList(int x, int y, int sizeX, int sizeY, char *text, int *scrollIndex, int *active){
    Rectangle rec = {x, y, sizeX, sizeY};
    return GuiListView(rec, text, scrollIndex, active);
}

int RayGUIDrawListEx(int x, int y, int sizeX, int sizeY, const char **text, int count, int *scrollIndex, int* focus, int *active){
    Rectangle rec = {x, y, sizeX, sizeY};
    return GuiListViewEx(rec, text, count, scrollIndex, active, focus);
}

int RayGUIDrawDialog(int x, int y, int sizeX, int sizeY, const char* title){
    Rectangle rec = {x, y, sizeX, sizeY};
    return GuiWindowBox(rec, title);
}

int RayGUIDrawDialogRec(Rectangle bounds, const char* title){
    return GuiWindowBox(bounds, title);
}

const char** RayGUITextSplit(const char* text, char deli, int* count, int* textrow){
    return GuiTextSplit(text, deli, count, textrow);
}

