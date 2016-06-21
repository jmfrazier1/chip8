#include "stdafx.h"
//#include "ViewPort.h"
using namespace std;

//#define SUPPRESS_ALL_OUTPUT

ViewPort::ViewPort()
{
     //bitMasks = { 0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1 };
     bitMasks[0] = { 0x80 };
     bitMasks[1] = { 0x40 };
     bitMasks[2] = { 0x20 };
     bitMasks[3] = { 0x10 };
     bitMasks[4] = { 0x08 };
     bitMasks[5] = { 0x04 };
     bitMasks[6] = { 0x02 };
     bitMasks[7] = { 0x01 };

     //bitMaskLeft = { 0x80, 0xC0, 0xE0, 0XF0, 0xF8, 0xFC, 0xFE, 0xFF };
     /*
     bitMaskLeft[0] = { 0x80 };
     bitMaskLeft[1] = { 0xC0 };
     bitMaskLeft[2] = { 0xE0 };
     bitMaskLeft[3] = { 0XF0 };
     bitMaskLeft[4] = { 0xF8 };
     bitMaskLeft[5] = { 0xFC };
     bitMaskLeft[6] = { 0xFE };
     bitMaskLeft[7] = { 0xFF };
     */

     bitMaskLeft[0] = { 0xFF }; //1111 1111
     bitMaskLeft[1] = { 0xFE }; //1111 1110
     bitMaskLeft[2] = { 0xFC }; //1111 1100
     bitMaskLeft[3] = { 0XF8 }; //1111 1000
     bitMaskLeft[4] = { 0xF0 }; //1111 0000
     bitMaskLeft[5] = { 0xE0 }; //1110 0000
     bitMaskLeft[6] = { 0xC0 }; //1100 0000
     bitMaskLeft[7] = { 0x80 }; //1000 0000

     /*
     vmBit  Mask            shift >>
     0  1111 1111 0xFF  0
     1  1111 1110 0xFE  1
     2  1111 1100 0xFC  2
     3  1111 1000 0xF8  3
     4  1111 0000 0xF0  4
     5  1110 0000 0xE0  5
     6  1100 0000 0xC0  6
     7  1000 0000 0x80  7
     */


     //bitMaskRight = { 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF };
     /*
     bitMaskRight[0] = { 0x01 };
     bitMaskRight[1] = { 0x03 };
     bitMaskRight[2] = { 0x07 };
     bitMaskRight[3] = { 0x0F };
     bitMaskRight[4] = { 0x1F };
     bitMaskRight[5] = { 0x3F };
     bitMaskRight[6] = { 0x7F };
     bitMaskRight[7] = { 0xFF };
     */

     bitMaskRight[0] = { 0x00 }; //0000 0000
     bitMaskRight[1] = { 0x01 }; //0000 0001
     bitMaskRight[2] = { 0x03 }; //0000 0011
     bitMaskRight[3] = { 0x07 }; //0000 0111
     bitMaskRight[4] = { 0x0F }; //0000 1111
     bitMaskRight[5] = { 0x1F }; //0001 1111
     bitMaskRight[6] = { 0x3F }; //0011 1111
     bitMaskRight[7] = { 0x7F }; //0111 1111
     //bitMaskRight[8] = { 0xFF };

     /*
     Right side
     vmBit  Mask            shift <<
     0  1111 1111 0xFF  0
     1  0111 1111 0x7F  1
     2  0011 1111 0x3F  2
     3  0001 1111 0x1F  3
     4  0000 1111 0x0F  4
     5  0000 0111 0x07  5
     6  0000 0011 0x03  6
     7  0000 0001 0x01  7
     */

#ifdef SUPPRESS_ALL_OUTPUT
     //Open output file
     //fopen_s(&SpriteOutputFile, "SpriteOutput.txt", "w");
#endif


}


ViewPort::~ViewPort()
{
#ifdef SUPPRESS_ALL_OUTPUT
     //Open output file
     //if (SpriteOutputFile != NULL)
     //{
     //     fclose(SpriteOutputFile);
     //     SpriteOutputFile = NULL;
     //}
#endif
}


void ViewPort::eraseViewPort()
{
#ifdef SUPPRESS_ALL_OUTPUT
     system("cls");
     //if (SpriteOutputFile != NULL)
     //{
     //     fprintf(SpriteOutputFile, "cls\n");
     //     fflush(SpriteOutputFile);
     //}
#endif
     //int i;
     //for (i = 0; i < (64 + 3)*32; i++);
     //{
     //     cout << "\b";
     //}
}

void ViewPort::drawToViewport()
{
     int i, j, b;
     eraseViewPort();
     for (i = 0; i < 32; i++)
     {
          char lineOut[65];
          memset(lineOut, 0, sizeof(lineOut));
          for (j = 0; j < 64 / 8; j++)
          {
               //Draw the byte videoMemory[i * 64 / 8 + j]
               //char byteOut[9];
               //memset(byteOut, 0, sizeof(byteOut));
               unsigned char mask = 0x80;
               for (b = 0; b < 8; b++)
               {
                    if ((videoMemory[i * 64 / 8 + j] & mask) != 0)
                    {
#ifdef SUPPRESS_ALL_OUTPUT
//                         cout << "X";
                         //byteOut[b] = 'X';
                         lineOut[j * 8 + b] = 'X';
#endif
                    }
                    else
                    {
#ifdef SUPPRESS_ALL_OUTPUT
//                         cout << " ";
                         //byteOut[b] = ' ';
                         lineOut[j * 8 + b] = ' ';
#endif
                    }
                    mask >>= 1;
               }
#ifdef SUPPRESS_ALL_OUTPUT
               //cout << byteOut;
#endif
          }
          //Emit new line
#ifdef SUPPRESS_ALL_OUTPUT
          cout << lineOut << endl;
          //cout << endl;
#endif
     }
}


void ViewPort::clearScreen()
{
     memset(videoMemory, 0, sizeof(videoMemory));
}

int ViewPort::isCollision(unsigned char x, unsigned char y)
{
     int collided = 0;
     int i;
     unsigned char start, end;
     unsigned char mask = 0x01;
     for (i = 0; i < 8 && !collided; i++)
     {    

          /*
          Truth table
          x  y  Collided
          0  0  0
          0  1  0 
          1  0  1 
          1  1  0 
          */

          start = x & mask;
          end = y & mask;
          //if (start == 0 && end == 0)
          //{
          //     collided = 1;
          //}
          //Causes immediate game over
          //if (start == 0 && end != 0)
          //{
          //     collided = 1;
          //}
          if (start != 0 && end == 0)
          {
               collided = 1;
          }
          //Added to hopefully get the collisions to work properly
          //if (start != 0 && end != 0)
          //{
          //     collided = 1;
          //}
          mask <<= 1;
     }

     return collided;
}

int ViewPort::drawSprite(unsigned short x, unsigned short y, short totalBytes, unsigned char *buffer)
{
     int erased = 0;
     int i;

     //Check for collision. If any of the bits in the box are set, it is a collsion.
     
     if (totalBytes == 1)
     {
          totalBytes = totalBytes;
     }

#ifndef SUPPRESS_ALL_OUTPUT
     cout << "Drawing sprite at (" << x << "," << y << ") spaning " << totalBytes << " bytes. [ ";

     for (i = 0; i < totalBytes; i++)
     {
          cout << hex << " " << (int)buffer[i];
     }

     cout << " ]" << endl;
#else
     //if (SpriteOutputFile != NULL)
     //{
     //     fprintf(SpriteOutputFile, "Drawing sprite at (%d,%d) spaning %d bytes. [ ", x, y, totalBytes);
     //     for (i = 0; i < totalBytes; i++)
     //     {
     //          fprintf(SpriteOutputFile, " %02x", (int)buffer[i]);
     //     }
     //     fprintf(SpriteOutputFile, " ]\n");
     //     fflush(SpriteOutputFile);
     //}
#endif

     for (i = 0; i < totalBytes; i++)
     {
          //videoMemory[]



          //For instance x = 12, y = 7; starts byte 57, bit 4
          int vmByte = ((y + i) * 8) + x / 8;
          int vmBit = x % 8;

          //Bounds check
          if (vmByte < 0 || vmByte > 64 / 8 * 32)
          {
               //Bounds check failure (Generally this would go beyond the video memory)
               break;
          }

          //First replacement is:
          //Bytes in first needed are 0000 1111 (0x0F) and second 1111 0000 (0xF0)
          //videoMemory[vmByte] = videoMemory[vmByte]

          /*
          Left side
          vmBit  Mask            shift >>
              0  1111 1111 0xFF  0
              1  1111 1110 0xFE  1
              2  1111 1100 0xFC  2
              3  1111 1000 0xF8  3
              4  1111 0000 0xF0  4
              5  1110 0000 0xE0  5
              6  1100 0000 0xC0  6
              7  1000 0000 0x80  7
          */

          /*
          Right side
          vmBit  Mask            shift <<
              0  1111 1111 0xFF  0
              1  0111 1111 0x7F  1
              2  0011 1111 0x3F  2
              3  0001 1111 0x1F  3
              4  0000 1111 0x0F  4
              5  0000 0111 0x07  5
              6  0000 0011 0x03  6
              7  0000 0001 0x01  7
          */

          //Replacement bits in proper location
          //unsigned char leftBitsInsert = (buffer[i] & bitMaskLeft[vmBit + 2]) >> (vmBit + 1);
          unsigned char leftBitsInsert = (buffer[i] & bitMaskLeft[vmBit]) >> (vmBit);
          
          //These are the bits in video memory which should be retained.
          unsigned char leftBitsRetain = videoMemory[vmByte] & bitMaskLeft[vmBit - 1];

          unsigned char newBits = videoMemory[vmByte] ^ leftBitsInsert;
          //XOR
          //0 0 0
          //0 1 1
          //1 0 1
          //1 1 0

          //mask to retain the current bits
          //if 0, mask should be 0
          //if 1, mask should be 0

#ifndef SUPPRESS_ALL_OUTPUT
          cout << "left from " << hex << (int)(videoMemory[vmByte]) << " to " << (int)newBits << endl;
#else
          //if (SpriteOutputFile != NULL)
          //{
          //     fprintf(SpriteOutputFile, "vmByte %d vmBit %d\n", vmByte, vmBit);
          //     fprintf(SpriteOutputFile, "Left from %02x to %02x\n", (int)(videoMemory[vmByte]), (int)newBits);
          //     fflush(SpriteOutputFile);
          //}
#endif

          //if (videoMemory[vmByte] != newBits)
          //{
          //     erased = 1;
          //}
          erased |= isCollision(videoMemory[vmByte], newBits);
          videoMemory[vmByte] = newBits;

          int secondVMByte = vmByte + 1;

          if (vmByte % 8 == 7)
          {
               secondVMByte = vmByte - 7;
          }


          if (vmBit != 0)
          {
               //A second byte must be changed

               //Replacement bits in proper location
               //unsigned char rightBitsInsert = (buffer[i] & bitMaskRight[vmBit]) << (8 - vmBit - 1);
               unsigned char rightBitsInsert = (buffer[i] & bitMaskRight[vmBit]) << (8 - vmBit);
               newBits = videoMemory[secondVMByte] ^ rightBitsInsert;
#ifndef SUPPRESS_ALL_OUTPUT
               cout << "right from " << hex << (int)(videoMemory[secondVMByte]) << " to " << (int)newBits << endl;
#else
               //if (SpriteOutputFile != NULL)
               //{
               //     fprintf(SpriteOutputFile, "Right from %02x to %02x\n", (int)(videoMemory[secondVMByte]), (int)newBits);
               //     fflush(SpriteOutputFile);
               //}
#endif
               erased |= isCollision(videoMemory[secondVMByte], newBits);
               //if (videoMemory[secondVMByte] != newBits)
               //{
               //     erased = 1;
               //}
               videoMemory[secondVMByte] = newBits;

          }

     }

     /*
     
     64 x 32
     0          1          2          3           4          5          6
     01234567 89012345 67890123 45678901 23456789 01234567 89012345 67890123

0    00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 0
8    00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 1
16   00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 2
24   00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 3
32   00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 4
40   00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 5
48   00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 6
56   00000000 0000x000 00000000 00000000 00000000 00000000 00000000 00000000 7

64   00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 8
72   00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 9
80   00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 10
88   00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 11
96   00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 12
104  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 13
112  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 14
120  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 15

128  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 16
136  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 17
144  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 18
152  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 19
160  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 20
168  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 21
176  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 22
184  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 23

192  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 24
200  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 25
208  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 26
216  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 27
224  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 28
232  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 29
240  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 30
248  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 31


     */


     /*
     
     Collision detection

     Original 0000 0000 new anything, no collision
     Original 0000 0001 new 0000 0000 Collision
                 not         
              1111 1110 XOR 0000 0000 
     Original 0000 0001 new 1111 1111 No Collision
     
     


     */


     return erased;
}


