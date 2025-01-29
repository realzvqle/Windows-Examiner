#ifndef DRAWING_H_INCLUDED
#define DRAWING_H_INCLUDED

#pragma once



#include "../superheader.h"

typedef enum ButtonState{
    BUTTON_NOT_INTERACTED,
    BUTTON_HOVER,
    BUTTON_CLICK_LEFT,
    BUTTON_CLICK_RIGHT,
    BUTTON_DOWN_LEFT,
    BUTTON_DOWN_RIGHT
} ButtonState;


void SetupDrawings();
void InsDrawText(const char* text, float posX, float posY, float size, Color color);
void CloseDrawings();
Font InsGetFont();
#endif