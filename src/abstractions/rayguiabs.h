#ifndef RAYGUIABS_H_INCLUDED
#define RAYGUIABS_H_INCLUDED






#include "../superheader.h"


int RayGUIDrawButton(int x, int y, int sizeX, int sizeY, const char* text);
int RayGUIDrawTextBox(int x, int y, int sizeX, int sizeY, char* text, int textSize, BOOL editMode);
void RayGUIInitialize();
int RayGUIDrawList(int x, int y, int sizeX, int sizeY, char *text, int *scrollIndex, int *active);
int RayGUIDrawDialog(int x, int y, int sizeX, int sizeY, const char* title);
int RayGUIDrawDialogRec(Rectangle bounds, const char* title);
#endif