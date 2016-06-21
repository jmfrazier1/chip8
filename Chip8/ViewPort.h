#pragma once
#include "stdafx.h"
class ViewPort
{
private:
     //The size is 64x32
     //4x5 pixel font (0-9, A-F)
     //drawing done is XOR
     //origin is upper left
     //8x1 to 15 sprites
     //when one or more pixels are erased while a sprite is drawn, the VF register is set to 0x01, otherwise it is set to 0x00
     unsigned char videoMemory[64/8 * 32];
     unsigned char bitMasks[8];
     unsigned char bitMaskLeft[8];// = { 0x80, 0xC0, 0xE0, 0XF0, 0xF8, 0xFC, 0xFE, 0xFF };
     unsigned char bitMaskRight[8];// = { 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF };

#ifdef SUPPRESS_ALL_OUTPUT
//     FILE *SpriteOutputFile;
#endif


public:
     ViewPort();
     ~ViewPort();
     void eraseViewPort();
     void drawToViewport();
     void clearScreen();
     int drawSprite(unsigned short x, unsigned short y, short totalBytes, unsigned char *buffer);
     static int isCollision(unsigned char x, unsigned char y);
};

