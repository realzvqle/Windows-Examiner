#ifndef RAYGUIABS_H_INCLUDED
#define RAYGUIABS_H_INCLUDED






#include "../superheader.h"


int RayGUIDrawButton(int x, int y, int sizeX, int sizeY, const char* text);
int RayGUIDrawTextBox(int x, int y, int sizeX, int sizeY, char* text, int textSize, BOOL editMode);
void RayGUIInitialize();


#endif