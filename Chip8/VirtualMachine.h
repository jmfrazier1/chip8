#pragma once
#include "stdafx.h"
//#include "Instruction.h"
#include "Register.h"
#include "Memory.h"
#include "ViewPort.h"

//#include <vector>
//#include <algorithm>
//#include <time.h>
//#include <stdlib.h>
//#include <sys/time.h>
//#include <Windows.h>
//#include <iostream>
//#include <conio.h>
using namespace std;


class VirtualMachine
{

private:
     //Stack - 16 levels
     int stackPointer = -1;
     unsigned short stack[16];

     //16 8-bit data registers (V0-VF)
     Register regs[16];

     //1 16-bit address register (I)
     unsigned short addr;

     //2 Timers
     Register soundTimer;
     Register delayTimer;

     //Program counter (instruction pointer)
     unsigned short pc;

     //instructionVector holds the instructions loaded from the ROM
     vector<Instruction> instructionVector;

     //4096 bytes of RAM
     Memory mem;

     //ViewPort
     ViewPort view;

     FILE *KeyPressOutputFile;
     FILE *InstructionOutputFile;


public:
     void loadROM(string romName);
#ifdef WONT_BE
     {
          FILE *romFile;
          errno_t retVal = fopen_s(&romFile, romName.c_str(), "rb");

          if (retVal != 0)
          {
               cout << "Failed to open the file: " << retVal << endl;
               return;
               //return 1;
          }
          cout << "Opened the file properly." << endl;

          while (true)
          {
               Instruction myInstr = Instruction::readNext(romFile);
               if (myInstr.isValid())
               {
                    //instructionList.insert(instructionList.end(), myInstr);
                    instructionVector.push_back(myInstr);
               }
               else if (myInstr.getType() != Instruction::TYPE_EOF)
               {
                    cout << "Invalid found at " << hex << myInstr.getMnemonic() << endl;
                    //break;
               }
               else
               {
                    break;
               }

          }

          vector<Instruction>::iterator it;

          //Second pass is used to rectify labels
          vector<short> dataItem;
          vector<short> labelItems;

          for (it = instructionVector.begin(); it < instructionVector.end(); it++ /*, i++*/)
          {
               Instruction ins = *it;
               string myMnemonic = ins.getMnemonic();
               //Find any data items referenced
               if (ins.isDataOperation())
               {
                    dataItem.push_back(ins.getNNN());
               }
               //dataItem
               //Find any labels we need to add
               if (ins.isLabelOperation())
               {
                    labelItems.push_back(ins.getNNN());
               }


               //cout << myMnemonic << endl;

          }

          //Sort and remove duplicates from the list of data items
          std::sort(dataItem.begin(), dataItem.end());
          dataItem.erase(unique(dataItem.begin(), dataItem.end()), dataItem.end());

          //Sort and remove duplicates from the list to labels
          std::sort(labelItems.begin(), labelItems.end());
          labelItems.erase(unique(labelItems.begin(), labelItems.end()), labelItems.end());


          vector<short>::iterator dataIt;
          for (dataIt = dataItem.begin(); dataIt < dataItem.end(); dataIt++ /*, i++*/)
          {
               int dataItm = *dataIt;


          }

          //Loop through all items which need labels, set the labels for the instructions.
          vector<short>::iterator labelIt;
          for (labelIt = labelItems.begin(); labelIt < labelItems.end(); labelIt++ /*, i++*/)
          {
               int dataItm = *labelIt;
               //cout << "One Label " << hex << dataItm << endl;

               for (it = instructionVector.begin(); it < instructionVector.end(); it++ /*, i++*/)
               {
                    Instruction ins = *it;
                    //cout << "Comparing " << ins.getMemoryAddress() << " to " << dataItm << endl;
                    if (ins.getMemoryAddress() == dataItm)
                    {
                         //cout << "Setting label" << endl;
                         //ins.setLabel("LABEL_" + Instruction::itostr(ins.getMemoryAddress(), 4));
                         (*it).setLabel("LABEL_" + Instruction::itostr(ins.getMemoryAddress(), 4) + ":");
                    }


               }

          }


          cout << "Listing mnemonics" << endl;
          //Loop through all instructions and modify the mnemonic to set the label to the value.
          for (it = instructionVector.begin(); it < instructionVector.end(); it++ /*, i++*/)
          {
               Instruction ins = *it;
               //if (ins.isLabelOperation())
               // {
               //     ins.setLabel("LABEL_" + Instruction::itostr(ins.getMemoryAddress(), 4));
               // }
               //cout << ins.getMnemonic() << endl;

               string label = ins.getLabel();
               //if (label.length() > 0)
               //{
               //     cout << "Label is " << label << endl;
               //}
               //else
               //{
               //     cout << "No label" << endl;
               //}

               cout << "Emit: "
                    << Instruction::itostr(ins.getMemoryAddress(), 4)
                    << "\t"
                    << Instruction::itostr(ins.getFullInstruction(), 4)
                    << "\t"
                    << ((ins.getLabel().length() > 0) ? ins.getLabel() + "\t" : "\t\t")
                    << ins.getMnemonic()
                    //               << " For " << &ins
                    << endl;

          }
          
          //Read ROM instructions directly into memory
          fseek(romFile, 0, SEEK_SET);
          
          int instr1;
          unsigned int currentLocation = 0x200;

          while (true)
          {
               instr1 = getc(romFile);
               if (instr1 == EOF)
               {
                    break;
               }
               mem.setMem(currentLocation++, instr1);
          }

          fclose(romFile);

          pc = 0x200;

     }
#endif

     void loadInterpreterSpace();
#ifdef WONT_BE
     {
          //Load 0
          mem.setMem(0x000, 0xF0);
          mem.setMem(0x001, 0x90);
          mem.setMem(0x002, 0x90);
          mem.setMem(0x003, 0x90);
          mem.setMem(0x004, 0xF0);

          //Load 1
          mem.setMem(0x005, 0x20);
          mem.setMem(0x006, 0x60);
          mem.setMem(0x007, 0x20);
          mem.setMem(0x008, 0x20);
          mem.setMem(0x009, 0x70);

          //Load 2
          mem.setMem(0x00A, 0xF0);
          mem.setMem(0x00B, 0x10);
          mem.setMem(0x00C, 0xF0);
          mem.setMem(0x00D, 0x80);
          mem.setMem(0x00E, 0xF0);

          //Load 3
          mem.setMem(0x00F, 0xF0);
          mem.setMem(0x010, 0x10);
          mem.setMem(0x011, 0xF0);
          mem.setMem(0x012, 0x10);
          mem.setMem(0x013, 0xF0);

          //Load 4
          mem.setMem(0x014, 0x90);
          mem.setMem(0x015, 0x90);
          mem.setMem(0x016, 0xF0);
          mem.setMem(0x017, 0x10);
          mem.setMem(0x018, 0x10);

          //Load 5
          mem.setMem(0x019, 0xF0);
          mem.setMem(0x01A, 0x80);
          mem.setMem(0x01B, 0xF0);
          mem.setMem(0x01C, 0x10);
          mem.setMem(0x01D, 0xF0);

          //Load 6
          mem.setMem(0x01E, 0xF0);
          mem.setMem(0x01F, 0x80);
          mem.setMem(0x020, 0xF0);
          mem.setMem(0x021, 0x90);
          mem.setMem(0x022, 0xF0);

          //Load 7
          mem.setMem(0x023, 0xF0);
          mem.setMem(0x024, 0x10);
          mem.setMem(0x025, 0x20);
          mem.setMem(0x026, 0x40);
          mem.setMem(0x027, 0x40);

          //Load 8
          mem.setMem(0x028, 0xF0);
          mem.setMem(0x029, 0x90);
          mem.setMem(0x02A, 0xF0);
          mem.setMem(0x02B, 0x90);
          mem.setMem(0x02C, 0xF0);

          //Load 9
          mem.setMem(0x02D, 0xF0);
          mem.setMem(0x02E, 0x90);
          mem.setMem(0x02F, 0xF0);
          mem.setMem(0x030, 0x10);
          mem.setMem(0x031, 0xF0);

          //Load A
          mem.setMem(0x032, 0xF0);
          mem.setMem(0x033, 0x90);
          mem.setMem(0x034, 0xF0);
          mem.setMem(0x035, 0x90);
          mem.setMem(0x036, 0x90);

          //Load B
          mem.setMem(0x037, 0xE0);
          mem.setMem(0x038, 0x90);
          mem.setMem(0x039, 0xE0);
          mem.setMem(0x03A, 0x90);
          mem.setMem(0x03B, 0xE0);

          //Load C
          mem.setMem(0x03C, 0xF0);
          mem.setMem(0x03D, 0x80);
          mem.setMem(0x03E, 0x80);
          mem.setMem(0x03F, 0x80);
          mem.setMem(0x040, 0xF0);

          //Load D
          mem.setMem(0x041, 0xE0);
          mem.setMem(0x042, 0x90);
          mem.setMem(0x043, 0x90);
          mem.setMem(0x044, 0x90);
          mem.setMem(0x045, 0xE0);

          //Load E
          mem.setMem(0x046, 0xF0);
          mem.setMem(0x047, 0x80);
          mem.setMem(0x048, 0xF0);
          mem.setMem(0x049, 0x80);
          mem.setMem(0x04A, 0xF0);

          //Load F
          mem.setMem(0x04B, 0xF0);
          mem.setMem(0x04C, 0x80);
          mem.setMem(0x04D, 0xF0);
          mem.setMem(0x04E, 0x80);
          mem.setMem(0x04F, 0x80);

     }
#endif

     int isKeyPressed(short key);
#ifdef WONT_BE
     {
          //Need to detect keys here
#ifndef SUPPRESS_ALL_OUTPUT
          cout << "Checking for key " << key << " being pressed" << endl;
#endif

          if (KeyPressOutputFile != NULL)
          {
               fprintf(KeyPressOutputFile, "Checking for key press %02x ", key);
               fflush(KeyPressOutputFile);
          }

          if (!_kbhit())
          {
               //No keys are pressed
               if (KeyPressOutputFile != NULL)
               {
                    fprintf(KeyPressOutputFile, " (No keys pressed) \n");
                    fflush(KeyPressOutputFile);
               }
               return 0;
          }
          //A key is pressed, check if it is the one we are looking for
          if (KeyPressOutputFile != NULL)
          {
               fprintf(KeyPressOutputFile, " ( a key is pressed )");
               fflush(KeyPressOutputFile);
          }
          switch (_getch())
          {
               case '1':
                    cout << "1 pressed" << endl;
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( key pressed is 1 )");
                         fflush(KeyPressOutputFile);
                    }
                    if (key = 0x1)
                    {
                         if (KeyPressOutputFile != NULL)
                         {
                              fprintf(KeyPressOutputFile, " ( Key matches )\n");
                              fflush(KeyPressOutputFile);
                         }
                         return 1;
                    }
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( Key does not match )\n");
                         fflush(KeyPressOutputFile);
                    }
                    break;
               case '2':
                    cout << "2 pressed" << endl;
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( key pressed is 2 )");
                         fflush(KeyPressOutputFile);
                    }
                    if (key = 0x2)
                    {
                         if (KeyPressOutputFile != NULL)
                         {
                              fprintf(KeyPressOutputFile, " ( Key matches )\n");
                              fflush(KeyPressOutputFile);
                         }
                         return 1;
                    }
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( Key does not match )\n");
                         fflush(KeyPressOutputFile);
                    }
                    break;
               case '3':
                    cout << "3 pressed" << endl;
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( key pressed is 3 )");
                         fflush(KeyPressOutputFile);
                    }
                    if (key = 0x3) 
                    {
                         if (KeyPressOutputFile != NULL)
                         {
                              fprintf(KeyPressOutputFile, " ( Key matches )\n");
                              fflush(KeyPressOutputFile);
                         }
                         return 1;
                    }
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( Key does not match )\n");
                         fflush(KeyPressOutputFile);
                    }
                    break;
               case '4':
                    cout << "4 pressed" << endl;
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( key pressed is 4 )");
                         fflush(KeyPressOutputFile);
                    }
                    if (key = 0x4) 
                    {
                         if (KeyPressOutputFile != NULL)
                         {
                              fprintf(KeyPressOutputFile, " ( Key matches )\n");
                              fflush(KeyPressOutputFile);
                         }
                         return 1;
                    }
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( Key does not match )\n");
                         fflush(KeyPressOutputFile);
                    }
                    break;
               case '5':
                    cout << "5 pressed" << endl;
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( key pressed is 5 )");
                         fflush(KeyPressOutputFile);
                    }
                    if (key = 0x5) 
                    {
                         if (KeyPressOutputFile != NULL)
                         {
                              fprintf(KeyPressOutputFile, " ( Key matches )\n");
                              fflush(KeyPressOutputFile);
                         }
                         return 1;
                    }
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( Key does not match )\n");
                         fflush(KeyPressOutputFile);
                    }
                    break;
               case '6':
                    cout << "6 pressed" << endl;
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( key pressed is 6 )");
                         fflush(KeyPressOutputFile);
                    }
                    if (key = 0x6) 
                    {
                         if (KeyPressOutputFile != NULL)
                         {
                              fprintf(KeyPressOutputFile, " ( Key matches )\n");
                              fflush(KeyPressOutputFile);
                         }
                         return 1;
                    }
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( Key does not match )\n");
                         fflush(KeyPressOutputFile);
                    }
                    break;
               case '7':
                    cout << "7 pressed" << endl;
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( key pressed is 7 )");
                         fflush(KeyPressOutputFile);
                    }
                    if (key = 0x7) 
                    {
                         if (KeyPressOutputFile != NULL)
                         {
                              fprintf(KeyPressOutputFile, " ( Key matches )\n");
                              fflush(KeyPressOutputFile);
                         }
                         return 1;
                    }
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( Key does not match )\n");
                         fflush(KeyPressOutputFile);
                    }
                    break;
               case '8':
                    cout << "8 pressed" << endl;
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( key pressed is 8 )");
                         fflush(KeyPressOutputFile);
                    }
                    if (key = 0x8) 
                    {
                         if (KeyPressOutputFile != NULL)
                         {
                              fprintf(KeyPressOutputFile, " ( Key matches )\n");
                              fflush(KeyPressOutputFile);
                         }
                         return 1;
                    }
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( Key does not match )\n");
                         fflush(KeyPressOutputFile);
                    }
                    break;
               case '9':
                    cout << "9 pressed" << endl;
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( key pressed is 9 )");
                         fflush(KeyPressOutputFile);
                    }
                    if (key = 0x9) 
                    {
                         if (KeyPressOutputFile != NULL)
                         {
                              fprintf(KeyPressOutputFile, " ( Key matches )\n");
                              fflush(KeyPressOutputFile);
                         }
                         return 1;
                    }
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( Key does not match )\n");
                         fflush(KeyPressOutputFile);
                    }
                    break;
               case '0':
                    cout << "0 pressed" << endl;
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( key pressed is 0 )");
                         fflush(KeyPressOutputFile);
                    }
                    if (key = 0x0) 
                    {
                         if (KeyPressOutputFile != NULL)
                         {
                              fprintf(KeyPressOutputFile, " ( Key matches )\n");
                              fflush(KeyPressOutputFile);
                         }
                         return 1;
                    }
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( Key does not match )\n");
                         fflush(KeyPressOutputFile);
                    }
                    break;
               case 'A':
               case 'a':
                    cout << "A pressed" << endl;
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( key pressed is A )");
                         fflush(KeyPressOutputFile);
                    }
                    if (key = 0xA) 
                    {
                         if (KeyPressOutputFile != NULL)
                         {
                              fprintf(KeyPressOutputFile, " ( Key matches )\n");
                              fflush(KeyPressOutputFile);
                         }
                         return 1;
                    }
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( Key does not match )\n");
                         fflush(KeyPressOutputFile);
                    }
                    break;
               case 'B':
               case 'b':
                    cout << "B pressed" << endl;
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( key pressed is B )");
                         fflush(KeyPressOutputFile);
                    }
                    if (key = 0xB) 
                    {
                         if (KeyPressOutputFile != NULL)
                         {
                              fprintf(KeyPressOutputFile, " ( Key matches )\n");
                              fflush(KeyPressOutputFile);
                         }
                         return 1;
                    }
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( Key does not match )\n");
                         fflush(KeyPressOutputFile);
                    }
                    break;
               case 'C':
               case 'c':
                    cout << "C pressed" << endl;
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( key pressed is C )");
                         fflush(KeyPressOutputFile);
                    }
                    if (key = 0xC) 
                    {
                         if (KeyPressOutputFile != NULL)
                         {
                              fprintf(KeyPressOutputFile, " ( Key matches )\n");
                              fflush(KeyPressOutputFile);
                         }
                         return 1;
                    }
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( Key does not match )\n");
                         fflush(KeyPressOutputFile);
                    }
                    break;
               case 'D':
               case 'd':
                    cout << "D pressed" << endl;
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( key pressed is D )");
                         fflush(KeyPressOutputFile);
                    }
                    if (key = 0xD) 
                    {
                         if (KeyPressOutputFile != NULL)
                         {
                              fprintf(KeyPressOutputFile, " ( Key matches )\n");
                              fflush(KeyPressOutputFile);
                         }
                         return 1;
                    }
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( Key does not match )\n");
                         fflush(KeyPressOutputFile);
                    }
                    break;
               case 'E':
               case 'e':
                    cout << "E pressed" << endl;
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( key pressed is E )");
                         fflush(KeyPressOutputFile);
                    }
                    if (key = 0xE) 
                    {
                         if (KeyPressOutputFile != NULL)
                         {
                              fprintf(KeyPressOutputFile, " ( Key matches )\n");
                              fflush(KeyPressOutputFile);
                         }
                         return 1;
                    }
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( Key does not match )\n");
                         fflush(KeyPressOutputFile);
                    }
                    break;
               case 'F':
               case 'f':
                    cout << "F pressed" << endl;
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( key pressed is F )");
                         fflush(KeyPressOutputFile);
                    }
                    if (key = 0xF) 
                    {
                         if (KeyPressOutputFile != NULL)
                         {
                              fprintf(KeyPressOutputFile, " ( Key matches )\n");
                              fflush(KeyPressOutputFile);
                         }
                         return 1;
                    }
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( Key does not match )\n");
                         fflush(KeyPressOutputFile);
                    }
                    break;
               default:
                    if (KeyPressOutputFile != NULL)
                    {
                         fprintf(KeyPressOutputFile, " ( Key pressed was other )\n");
                         fflush(KeyPressOutputFile);
                    }

          }
          return 0;
     }
#endif

     int isKeyUp(short key);
#ifdef WONT_BE
     {
          //Need to detect keys here
#ifndef SUPPRESS_ALL_OUTPUT
          cout << "Checking for key " << key << " being up" << endl;
#endif

          if (KeyPressOutputFile != NULL)
          {
               fprintf(KeyPressOutputFile, "Checking for key up %02x\n", key);
               fflush(KeyPressOutputFile);
          }


          if (!_kbhit())
          {
               //No keys are pressed
               return 1;
          }
          //return 0;
          switch (_getch())
          {
          case '1':
               cout << "1 pressed" << endl;
               if (key != 0x1)
               {
                    return 1;
               }
               break;
          case '2':
               cout << "2 pressed" << endl;
               if (key != 0x2)
                    return 1;
               break;
          case '3':
               cout << "3 pressed" << endl;
               if (key != 0x3) 
                    return 1;
               break;
          case '4':
               cout << "4 pressed" << endl;
               if (key != 0x4) 
                    return 1;
               break;
          case '5':
               cout << "5 pressed" << endl;
               if (key != 0x5)
                    return 1;
               break;
          case '6':
               cout << "6 pressed" << endl;
               if (key != 0x6)
                    return 1;
               break;
          case '7':
               cout << "7 pressed" << endl;
               if (key != 0x7)
                    return 1;
               break;
          case '8':
               cout << "8 pressed" << endl;
               if (key != 0x8) 
                    return 1;
               break;
          case '9':
               cout << "9 pressed" << endl;
               if (key != 0x9) 
                    return 1;
               break;
          case '0':
               cout << "0 pressed" << endl;
               if (key != 0x0)
                    return 1;
               break;
          case 'A':
          case 'a':
               cout << "A pressed" << endl;
               if (key != 0xA)
                    return 1;
               break;
          case 'B':
          case 'b':
               cout << "B pressed" << endl;
               if (key != 0xB)
                    return 1;
               break;
          case 'C':
          case 'c':
               cout << "C pressed" << endl;
               if (key != 0xC) 
                    return 1;
               break;
          case 'D':
          case 'd':
               cout << "D pressed" << endl;
               if (key != 0xD) 
                    return 1;
               break;
          case 'E':
          case 'e':
               cout << "E pressed" << endl;
               if (key != 0xE) 
                    return 1;
               break;
          case 'F':
          case 'f':
               cout << "F pressed" << endl;
               if (key != 0xF) 
                    return 1;
               break;
          default:
               return 1;
          }
          return 0;
     }
#endif

     //This needs to work on the key press without needing to have enter pressed.
     //Also only respond to the requisite key presses.
     int waitForKeyPress();
#ifdef WONT_BE
     {
          int keyPressed = getchar();
          //switch (keyPressed)
          //{    
          //     case :
          //     default:
          //          break;
          //}
#ifndef SUPPRESS_ALL_OUTPUT
          cout << "Key press is " << keyPressed << endl;
#endif
          return keyPressed;
     }
#endif

     unsigned short getRandomByte();
#ifdef WONT_BE
     {
          unsigned short ret = 0;
          //generates a random number from 0 to 255
          int r = rand();
          ret = r % 255;
          return ret;
     }
#endif

//#define WAIT_FREQ_TIME 10
//#define WAIT_FREQ_TIME 00

     void breakOnInstruction(unsigned int fullInstruction, unsigned short _pc)
     {
          switch (fullInstruction)
          {
               case 0x1217: 
                    fullInstruction = fullInstruction; 
                    break;//      JMP 0217
               case 0xa341: 
                    fullInstruction = fullInstruction; 
                    break;//      I = 0x341
               case 0x6004: 
                    fullInstruction = fullInstruction; 
                    break;//      LD V0, 0x04
               case 0x6109: 
                    fullInstruction = fullInstruction; 
                    break;//      LD V1, 0x09
               case 0x620e: 
                    fullInstruction = fullInstruction; 
                    break;//      LD V2, 0x0E
               case 0x6704: 
                    fullInstruction = fullInstruction; 
                    break;//      LD V7, 0x04
               case 0xd01e: 
                    fullInstruction = fullInstruction; 
                    break;//      DRW V0, V1, 0xE bytes
               case 0xf21e: 
                    if (_pc == 0x0223)
                    {
                         fullInstruction = fullInstruction;
                    }
                    else if (_pc == 0x02cf)
                    {
                         fullInstruction = fullInstruction;
                    }
                    else if (_pc == 0x02e3)
                    {
                         fullInstruction = fullInstruction;
                    }
                    break;//      I = I + V2
               case 0x700c: 
                    fullInstruction = fullInstruction; 
                    break;//      V0 = V0 + 0x0C
               case 0x3040: 
                    fullInstruction = fullInstruction; 
                    break;//      Skip next if V0 = 0x40 (64)
               case 0x1221: 
                    fullInstruction = fullInstruction; 
                    break;//      JMP 0221
               case 0xf00a: 
                    if (_pc == 0x022b)
                    {
                         fullInstruction = fullInstruction;
                    }
                    else if (_pc == 0x0231)
                    {
                         fullInstruction = fullInstruction;
                    }
                    break;//      LD V0, K (KeyPress wait)
               case 0x00e0: 
                    if (_pc == 0x022d)
                    {
                         fullInstruction = fullInstruction;
                    }
                    else if (_pc == 0x0233)
                    {
                         fullInstruction = fullInstruction;
                    }
                    else if (_pc == 0x02bb)
                    {
                         fullInstruction = fullInstruction;
                    }
                    break;//      CLS
               case 0x22d9: 
                    fullInstruction = fullInstruction; 
                    break;//      CALL 0x2D9
               case 0x8e70: 
                    fullInstruction = fullInstruction; 
                    break;//      VE = V7
               case 0xa31e: 
                    if (_pc == 0x0237)
                    {
                         fullInstruction = fullInstruction;
                    }
                    else if (_pc == 0x0281)
                    {
                         fullInstruction = fullInstruction;
                    }
                    else if (_pc == 0x0295)
                    {
                         fullInstruction = fullInstruction;
                    }
                    break;//      I = 0x31E
               case 0x6b1f: 
                    fullInstruction = fullInstruction; 
                    break;//      VB = 1F
               case 0xcc1f: 
                    fullInstruction = fullInstruction; 
                    break;//      VC = random byte AND 0x1F
               case 0x8cc4: 
                    fullInstruction = fullInstruction; 
                    break;//      VC = VC + VC; VF = Carry
               case 0xdcb2: 
                    if (_pc == 0x023f)
                    {
                         fullInstruction = fullInstruction; //Tower start attempt
                    }
                    else if (_pc == 0x0245)
                    {
                         fullInstruction = fullInstruction; //Erases a tower part when a collision has occurred
                    }
                    else if (_pc == 0x024f)
                    {
                         fullInstruction = fullInstruction; //Draws the rest of the tower
                    }
                    break;//      DRW VC, VB, 2 Bytes
               case 0x3f01: 
                    fullInstruction = fullInstruction; 
                    break;//      Skip next if VF = 0x01
               case 0x1249: 
                    fullInstruction = fullInstruction; 
                    break;//      JMP 0249 ;No collision while starting tower, so continue building tower
               case 0x1239: 
                    if (_pc == 0x0247)
                    {
                         fullInstruction = fullInstruction;
                    }
                    else if (_pc == 0x025b)
                    {
                         fullInstruction = fullInstruction;
                    }
                    break;//      JMP 0239
               case 0xca07: 
                    //This happens when a successful start of a tower has been placed without collision.
                    fullInstruction = fullInstruction; 
                    break;//      VA = random byte AND 0x07
               case 0x7a01: 
                    fullInstruction = fullInstruction; 
                    break;//      VA = VA + 0x01
               case 0x7bfe: 
                    fullInstruction = fullInstruction; 
                    break;//      VB = VB + 0xFE
               case 0x7aff: 
                    fullInstruction = fullInstruction; 
                    break;//      VA = VA + 0xFF
               case 0x3a00: 
                    fullInstruction = fullInstruction; 
                    break;//      Skip next if VA = 00
               case 0x124d: 
                    fullInstruction = fullInstruction; 
                    break;//      JMP 024D
               case 0x7eff: 
                    fullInstruction = fullInstruction; 
                    break;//      VE = VE + 0xFF
               case 0x3e00: 
                    fullInstruction = fullInstruction; 
                    break;//      Skip next if VE = 0x00
               case 0x6b00: 
                    if (_pc == 0x025d)
                    {
                         fullInstruction = fullInstruction;
                    }
                    else if (_pc == 0x02a1)
                    {
                         fullInstruction = fullInstruction;
                    }
                    break;//      VB = 0x00
               case 0x8c70: 
                    fullInstruction = fullInstruction; 
                    break;//      VC = V7
               case 0x6d00: 
                    if (_pc == 0x0261)
                    {
                         fullInstruction = fullInstruction;
                    }
                    else if (_pc == 0x02b5)
                    {
                         fullInstruction = fullInstruction;
                    }
                    break;//      VD = 0x00
               case 0x6e00: 
                    fullInstruction = fullInstruction; 
                    break;//      VE = 0x00
               case 0xa31b: 
                    if (_pc == 0x0265)
                    {
                         fullInstruction = fullInstruction;
                    }
                    else if (_pc == 0x02ab)
                    {
                         fullInstruction = fullInstruction;
                    }
                    else if (_pc == 0x02c1)
                    {
                         fullInstruction = fullInstruction;
                    }
                    break;//      I = 0x31B
               case 0xdde3: 
                    if (_pc == 0x0267)
                    {
                         fullInstruction = fullInstruction;
                    }
                    else if (_pc == 0x02ad)
                    {
                         fullInstruction = fullInstruction;
                    }
                    else if (_pc == 0x02c3)
                    {
                         fullInstruction = fullInstruction;
                    }
                    break;//      DRW VD, VE, 3 Bytes
               case 0x3f00: 
                    fullInstruction = fullInstruction; 
                    break;//      skip next if VF == 0x00
               case 0x12c1: 
                    fullInstruction = fullInstruction; 
                    break;// 		JMP 02c1 ;Jump upon collision while drawing the plane
               case 0x3b00: 
                    fullInstruction = fullInstruction; 
                    break;//      skip next if VB == 0x00
               case 0x1281: 
                    fullInstruction = fullInstruction; 
                    break;// 		JMP 0281 ;VB == 1 when a bomb is dropping
               case 0x6005: 
                    if (_pc == 0x0271)
                    {
                         fullInstruction = fullInstruction;
                    }
                    else if (_pc == 0x0287)
                    {
                         fullInstruction = fullInstruction;
                    }
                    break;//      V0 = 0x05
               case 0xe09e: 
                    fullInstruction = fullInstruction; 
                    break;//      SKP Next if V0 is pressed
               case 0x1287: 
                    fullInstruction = fullInstruction; 
                    break;//      JMP 0287 ;Jump if the 5 key is not pressed, normally it will not be
               case 0x6b01: 
                    fullInstruction = fullInstruction; 
                    break;//      VB = 0x01
               case 0x88d0: 
                    fullInstruction = fullInstruction; 
                    break;//      V8 = VD
               case 0x7802: 
                    fullInstruction = fullInstruction; 
                    break;//      V8 = V8 + 0x02
               case 0x89e0: 
                    fullInstruction = fullInstruction; 
                    break;//      V9 = VE
               case 0x7903: 
                    fullInstruction = fullInstruction; 
                    break;//      V9 = V9 + 0x03
               case 0xd891: 
                    if (_pc == 0x0283)
                    {
                         fullInstruction = fullInstruction;
                    }
                    else if (_pc == 0x0299)
                    {
                         fullInstruction = fullInstruction;
                    }
                    break;//      DRW V8, V9, 1 Byte
               case 0x81f0: 
                    fullInstruction = fullInstruction; 
                    break;//      V1 = VF
               case 0xf015: 
                    fullInstruction = fullInstruction; 
                    break;//      delay timer = V0
               case 0xf007: 
                    fullInstruction = fullInstruction; 
                    break;//      V0 = delay timer
               case 0x3000: 
                    fullInstruction = fullInstruction; 
                    break;//      Skip next if V0 = 0x00
               case 0x128b: 
                    fullInstruction = fullInstruction; 
                    break;// 		JMP 028b ;Loops back while the delay timer is not 0
               case 0x3b01: 
                    fullInstruction = fullInstruction; 
                    break;//      Skip next if VB = 0x01 
               case 0x12ab: 
                    if (_pc == 0x0293)
                    {
                         fullInstruction = fullInstruction;
                    }
                    else if (_pc == 0x029f)
                    {
                         fullInstruction = fullInstruction;
                    }
                    break;//      JMP 02ab ;Bomb is not dropping
               case 0x3101: 
                    fullInstruction = fullInstruction; 
                    break;//      Skip next if V1 = 0x01
               case 0x7901: 
                    fullInstruction = fullInstruction; 
                    break;//      V9 = V9 + 0x01
               case 0x3920: 
                    fullInstruction = fullInstruction; 
                    break;//      Skip next if V9 = 0x20
               case 0x3100: 
                    fullInstruction = fullInstruction;	
                    break;//	Skip next instruction if V1 = 0x00
               case 0x7cff: 
                    fullInstruction = fullInstruction;	
                    break;//	Vc = Vc + 0xff
               case 0x4c00: 
                    fullInstruction = fullInstruction;	
                    break;//	Skip next if Vc != 0x00
               case 0x12bb: 
                    fullInstruction = fullInstruction;	
                    break;//	JMP 02bb
               case 0x7d02: 
                    fullInstruction = fullInstruction; 
                    break;//      VD = VD + 0x02
               case 0x3d40: 
                    fullInstruction = fullInstruction; 
                    break;//      Skip next if VD = 0x40
               case 0x12b9: 
                    fullInstruction = fullInstruction; 
                    break;//      JMP 02b9
               case 0x7e01: 
                    fullInstruction = fullInstruction;	
                    break;//	Ve = Ve + 0x01
               case 0x1265: 
                    fullInstruction = fullInstruction; 
                    break;//      JMP 0265
               case 0x7702: 
                    fullInstruction = fullInstruction;	
                    break;//	V7 = V7 + 0x02
               case 0x122d: 
                    fullInstruction = fullInstruction; 
                    break;// 		JMP 022d
               case 0x6014: 
                    fullInstruction = fullInstruction;	
                    break;//	V0 = 0x14
               case 0x6102: 
                    fullInstruction = fullInstruction;	
                    break;//	V1 = 0x02
               case 0x620b: 
                    fullInstruction = fullInstruction;	
                    break;//	V2 = 0x0b
               case 0xa320: 
                    fullInstruction = fullInstruction;	
                    break;//	I = 0x320
               case 0xd01b: 
                    fullInstruction = fullInstruction;	
                    break;//	DRW V0, V1, 0xb bytes
               case 0x7008: 
                    if (_pc == 0x02d1)
                    {
                         fullInstruction = fullInstruction;
                    }
                    else if (_pc == 0x02e5)
                    {
                         fullInstruction = fullInstruction;
                    }
                    break;//	V0 = V0 + 0x08
               case 0x302c: 
                    fullInstruction = fullInstruction;	
                    break;//	Skip next if V0 = 0x2c
               case 0x12cd: 
                    fullInstruction = fullInstruction; 
                    break;// 		JMP 02cd
               case 0x12d7: 
                    fullInstruction = fullInstruction; 
                    break;// 		JMP 02d7		
               case 0x600a: 
                    fullInstruction = fullInstruction; 
                    break;//      V0 = 0x0A
               case 0x610d: 
                    if (_pc == 0x02db)
                    {
                         fullInstruction = fullInstruction;
                    }
                    else if (_pc == 0x02fb)
                    {
                         fullInstruction = fullInstruction;
                    }
                    break;//      V1 = 0x0D
               case 0x6205: 
                    fullInstruction = fullInstruction; 
                    break;//      V2 = 0x05
               case 0xa307: 
                    fullInstruction = fullInstruction; 
                    break;//      I = 0x307
               case 0xd015: 
                    if (_pc == 0x02e1)
                    {
                         fullInstruction = fullInstruction;
                    }
                    else if (_pc == 0x02fd)
                    {
                         fullInstruction = fullInstruction;
                    }
                    else if (_pc == 0x0303)
                    {
                         fullInstruction = fullInstruction;
                    }
                    break;//      DRW V0, V1, 0x5 bytes
               case 0x302a: 
                    fullInstruction = fullInstruction; 
                    break;//      Skip next if V0 = 2a
               case 0x12e1: 
                    fullInstruction = fullInstruction; 
                    break;//      JMP 02e1
               case 0x8070: 
                    fullInstruction = fullInstruction; 
                    break;//      V0 = V7
               case 0x70fe: 
                    fullInstruction = fullInstruction; 
                    break;//      V0 = V0 + 0xFE
               case 0x8006: 
                    fullInstruction = fullInstruction; 
                    break;//      V0 = V0 >> 1
               case 0xa387: 
                    fullInstruction = fullInstruction; 
                    break;//      I = 0x387
               case 0xf033: 
                    fullInstruction = fullInstruction; 
                    break;//      I = BCD(V0)
               case 0xf265: 
                    fullInstruction = fullInstruction; 
                    break;//      LD V0 - V2 From [I]
               case 0x602d: 
                    fullInstruction = fullInstruction; 
                    break;//      V0 = 0x2D
               case 0xf129: 
                    fullInstruction = fullInstruction; 
                    break;//      I = LOC(Sprite V1)
               case 0x7005: 
                    fullInstruction = fullInstruction; 
                    break;//      V0 = V0 + 0x05
               case 0xf229: 
                    fullInstruction = fullInstruction; 
                    break;//      I = LOC(Sprite V2)
               case 0x00ee: 
                    fullInstruction = fullInstruction; 
                    break;//      RET
          }

     }


     void run();
#ifdef WONT_BE
     {
          int x;
          //int kk;
          int i; //Loop control variable

          unsigned char spriteBuffer[16];
          //long currStartTime;
          //struct timeval tp;
          //gettimeofday(&tp, NULL);
          //long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;          //Main loop
          
          //struct tm * timeinfo;
          //time_t lastStart;
          //time_t currentTime;

          //time(&lastStart);

          srand((unsigned int)time(NULL));


          while (true)
          {
               
               ////currStartTime = time();
               //time(&currentTime);

               //if (lastStart + (WAIT_FREQ_TIME) > currentTime)
               //{
               //     //Do wait
               //     DWORD timeDelay = (DWORD)(lastStart + WAIT_FREQ_TIME - currentTime);
               //     Sleep(timeDelay);
               //}
               //lastStart = currentTime;

               //Get current instruction based on program counter
               //unsigned int instr1 = mem.getMem(pc);
               //unsigned int instr2 = mem.getMem(pc + 1);

               //unsigned int fullInstruction = (instr1 << 8) + instr2;

               unsigned int fullInstruction = (mem.getMem(pc) << 8) + mem.getMem(pc + 1);

               if (InstructionOutputFile != NULL)
               {
                    fprintf(InstructionOutputFile, "%04x:%04x\n", pc, fullInstruction);
                    fflush(InstructionOutputFile);
               }

               Instruction currentInstruction(0, fullInstruction);


               breakOnInstruction(fullInstruction, pc);

               //cout << "Stack size " << dec << stackPointer << endl;

               switch (currentInstruction.getType())
               {
                         case Instruction::TYPE_CLS:
                              view.clearScreen();
                              pc += 2;
                              break;

                         case Instruction::TYPE_MOV:
                              //LD Vx, kk
                              //x = currentInstruction.getX();
                              //kk = currentInstruction.getKK();
                              //
                              //regs[x].setValue(kk);

                              regs[currentInstruction.getX()].setValue((unsigned char)currentInstruction.getKK());
                              pc += 2;
                              break;
                         case Instruction::TYPE_SKNE_REG:
                              //Skip next instruction if Vx != Vy
                              if (regs[currentInstruction.getX()].getValue() != regs[currentInstruction.getY()].getValue())
                              {
                                   pc += 2;
                              }
                              pc += 2;
                              break;
                         case Instruction::TYPE_SKNE: //B   4XKK
                              //Skip next instruction if Vx != kk.

                              //The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
                              if (regs[currentInstruction.getX()].getValue() != currentInstruction.getKK())
                              {
                                   pc += 2;
                              }
                              pc += 2;
                              break;
                         case Instruction::TYPE_MVI:
                              addr = currentInstruction.getNNN();
                              pc += 2;
                              break;
                         case Instruction::TYPE_STR:
                              //Store registers V0 through Vx in memory starting at location I.
                              //The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
                              x = currentInstruction.getX();
                              for (i = 0; i <= x; i++)
                              {
                                   mem.setMem(addr + i, regs[i].getValue());
                              }
                              pc += 2;
                              break;
                         case Instruction::TYPE_LDR:
                              //Read registers V0 through Vx from memory starting at location I.
                              //The interpreter reads values from memory starting at location I into registers V0 through Vx.
                              x = currentInstruction.getX();
                              for (i = 0; i <= x; i++)
                              {
                                   regs[i].setValue(mem.getMem(addr + i));
                              }
                              pc += 2;
                              break;
                         case Instruction::TYPE_JSR:
                              //Call subroutine at nnn.
                              //The interpreter increments the stack pointer, then puts the current PC on the top of the stack.The PC is then set to nnn.
                              pc += 2;
                              //cout << "Subroutine, return point should be " << hex << pc << endl;
                              stack[++stackPointer] = pc;
                              pc = currentInstruction.getNNN();
                              break;
                         case Instruction::TYPE_SKEQ: //A   3XKK
                              //Skip next instruction if Vx = kk.
                              //The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
                              if (regs[currentInstruction.getX()].getValue() == currentInstruction.getKK())
                              {
                                   pc += 2;
                              }
                              pc += 2;
                              break;
                         case Instruction::TYPE_SKEQ_REG:
                              if (regs[currentInstruction.getX()].getValue() == regs[currentInstruction.getY()].getValue())
                              {
                                   pc += 2;
                              }
                              pc += 2;
                              break;
                         case Instruction::TYPE_RTS:
                              //Return from a subroutine.
                              //The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
                              pc = stack[stackPointer--];
                              //cout << "Returning to " << hex << pc << endl;
                              break;
                         case Instruction::TYPE_ADD:
                              //Set Vx = Vx + kk.
                              //Adds the value kk to the value of register Vx, then stores the result in Vx.
                              regs[currentInstruction.getX()].setValue(regs[currentInstruction.getX()].getValue() + currentInstruction.getKK());
                              pc += 2;
                              break;
                         case Instruction::TYPE_AND:
                              //Set Vx = Vx AND Vy.
                              //Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx.
                              //A bitwise AND compares the corrseponding bits from two values, and if both bits are 1, 
                              //then the same bit in the result is also 1. Otherwise, it is 0.
                              regs[currentInstruction.getX()].setValue(regs[currentInstruction.getX()].getValue() & regs[currentInstruction.getY()].getValue());
                              pc += 2;
                              break;

                         //Keyboard instructions
                         case Instruction::TYPE_SKPR:
                              //Skip next instruction if key with the value of Vx is pressed.
                              //Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.
                              if (isKeyPressed(regs[currentInstruction.getX()].getValue()))
                              {
                                   pc += 2;
                              }
                              pc += 2;
                              break;
                         case Instruction::TYPE_SKUP:
                              //Skip next instruction if key with the value of Vx is not pressed.
                              //Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.
                              if (isKeyUp(regs[currentInstruction.getX()].getValue()))
                              {
                                   pc += 2;
                              }
                              //cout << "pc " << hex << pc << endl;
                              pc += 2;
                              break;
                         case Instruction::TYPE_KEY: //20
                              //Wait for a key press, store the value of the key in Vx.
                              //All execution stops until a key is pressed, then the value of that key is stored in Vx.
                              regs[currentInstruction.getX()].setValue(waitForKeyPress());
                              pc += 2;
                              break;



                         case Instruction::TYPE_JMP:
                              //Jump to location nnn.
                              //The interpreter sets the program counter to nnn.
                              pc = currentInstruction.getNNN();
#ifndef SUPPRESS_ALL_OUTPUT
                              cout << "Jumping to " << hex << pc << endl;
#endif
                              break;
                         case Instruction::TYPE_JMI: //1A   BNNN
                              //Jump to location nnn + V0.
                              //The program counter is set to nnn plus the value of V0.
                              pc = regs[0].getValue() + currentInstruction.getNNN();
#ifndef SUPPRESS_ALL_OUTPUT
                              cout << "Jumping to " << hex << pc << endl;
#endif
                              break;
                         case Instruction::TYPE_ADI:
                              //Set I = I + Vx.
                              //The values of I and Vx are added, and the results are stored in I.
                              addr += regs[currentInstruction.getX()].getValue();
                              pc += 2;
                              break;
                         case Instruction::TYPE_MOV_REG:
                              //Set Vx = Vy.
                              //Stores the value of register Vy in register Vx.
                              regs[currentInstruction.getX()].setValue(regs[currentInstruction.getY()].getValue());
                              pc += 2;
                              break;
                         case Instruction::TYPE_ADD_REG:
                              //Set Vx = Vx + Vy, set VF = carry.
                              //The values of Vx and Vy are added together.If the result is greater than 8 bits(i.e., > 255, ) 
                              //VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
                              x = regs[currentInstruction.getX()].getValue() + regs[currentInstruction.getY()].getValue();
                              regs[0xF].setValue((x > 255) ? 1 : 0);
                              regs[currentInstruction.getX()].setValue(x & 0xFF);
                              pc += 2;
                              break;
                         case Instruction::TYPE_SPRITE: //1C
                              //Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
                              //The interpreter reads n bytes from memory, starting at the address stored in I. These bytes 
                              //are then displayed as sprites on screen at coordinates(Vx, Vy).Sprites are XORed onto the 
                              //existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is 
                              //set to 0. If the sprite is positioned so part of it is outside the coordinates of the 
                              //display, it wraps around to the opposite side of the screen.See instruction 8xy3 for more 
                              //information on XOR, and section 2.4, Display, for more information on the Chip - 8 screen and sprites.
                              //cout << "X: " << currentInstruction.getX() << "  Y: " << currentInstruction.getY() << " N: " << currentInstruction.getN() << endl;

                              //Where to start reading
                              //mem.getMem(addr)
                              memset(spriteBuffer, 0, sizeof(spriteBuffer));

                              for (i = 0; i < currentInstruction.getN(); i++)
                              {
                                   spriteBuffer[i] = mem.getMem(addr + i);
                              }

                              if (view.drawSprite(regs[currentInstruction.getX()].getValue(), regs[currentInstruction.getY()].getValue(), currentInstruction.getN(), spriteBuffer))
                              {
                                   regs[0xF].setValue(1);
                              }
                              else
                              {
                                   regs[0xF].setValue(0);
                              }
                              pc += 2;
                              view.drawToViewport();
                              //cout << (int)regs[currentInstruction.getX()].getValue() << " " << (int)regs[currentInstruction.getY()].getValue() << " Collision: " << (int)regs[0xF].getValue() << endl;
                              //exit(2);
                              break;
                         case Instruction::TYPE_FONT: //24
                              //Set I = location of sprite for digit Vx.
                              //The value of I is set to the location for the hexadecimal sprite corresponding to the 
                              //value of Vx. See section 2.4, Display, for more information on the Chip - 8 hexadecimal font.
                              addr = 0x000 + (regs[currentInstruction.getX()].getValue() * 5);
                              pc += 2;
                              break;
                         case Instruction::TYPE_SDELAY: //21
                              //Set delay timer = Vx.
                              //DT is set equal to the value of Vx.
                              delayTimer.setValue(regs[currentInstruction.getX()].getValue());
                              pc += 2;
                              break;
                         case Instruction::TYPE_GDELAY: //1F
                              //Set Vx = delay timer value.
                              //The value of DT is placed into Vx.
                              //delayTimer.setValue((unsigned char)currentInstruction.getX());
                              regs[currentInstruction.getX()].setValue(delayTimer.getValue());
                              pc += 2;
                              break;
                         case Instruction::TYPE_GSOUND: //22
                              //Set sound timer = Vx.
                              //ST is set equal to the value of Vx.
                              soundTimer.setValue(regs[currentInstruction.getX()].getValue());
                              pc += 2;
                              break;
                         case Instruction::TYPE_XOR: //12
                              //Set Vx = Vx XOR Vy.
                              //Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx.
                              //An exclusive OR compares the corrseponding bits from two values, and if the bits are not 
                              //both the same, then the corresponding bit in the result is set to 1. Otherwise, it is 0.
                              regs[currentInstruction.getX()].setValue(regs[currentInstruction.getX()].getValue() ^ regs[currentInstruction.getY()].getValue());
                              pc += 2;
                              break;
                         case Instruction::TYPE_SHR: //15
                              //Set Vx = Vx SHR 1.
                              //If the least - significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
                              regs[0xF].setValue(regs[currentInstruction.getX()].getValue() & 0x1);
                              regs[currentInstruction.getX()].setValue(regs[currentInstruction.getX()].getValue() >> 1);
                              pc += 2;
                              break;
                         case Instruction::TYPE_BCD: //25
                              //Store BCD representation of Vx in memory locations I, I+1, and I+2.
                              //The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location 
                              //in I, the tens digit at location I + 1, and the ones digit at location I + 2.
                              mem.setMem(addr + 0, regs[currentInstruction.getX()].getValue() / 100);
                              mem.setMem(addr + 1, (regs[currentInstruction.getX()].getValue() % 100) / 10);
                              mem.setMem(addr + 2, regs[currentInstruction.getX()].getValue() % 10);
                              pc += 2;
                              break;
                         case Instruction::TYPE_RAND: //1B
                              //Set Vx = random byte AND kk.
                              //The interpreter generates a random number from 0 to 255, which is then ANDed with the 
                              //value kk.The results are stored in Vx. See instruction 8xy2 for more information on AND.
                              regs[currentInstruction.getX()].setValue(getRandomByte() & currentInstruction.getKK());
                              pc += 2;
                              break;
                         case Instruction::TYPE_SUB: //14
                              //Set Vx = Vx - Vy, set VF = NOT borrow.
                              //If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
                              regs[0xF].setValue((regs[currentInstruction.getX()].getValue() > regs[currentInstruction.getY()].getValue()) ? 1 : 0);
                              regs[currentInstruction.getX()].setValue(regs[currentInstruction.getX()].getValue() - regs[currentInstruction.getY()].getValue());
                              pc += 2;
                              break;
                         case Instruction::TYPE_RSB: //16   8XY7
                              //Set Vx = Vy - Vx, set VF = NOT borrow.
                              //If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
                              regs[0xF].setValue((regs[currentInstruction.getY()].getValue() > regs[currentInstruction.getX()].getValue()) ? 1 : 0);
                              regs[currentInstruction.getX()].setValue(regs[currentInstruction.getY()].getValue() - regs[currentInstruction.getX()].getValue());
                              pc += 2;
                              break;
                         case Instruction::TYPE_OR: //10
                              //Set Vx = Vx OR Vy.
                              //Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx.
                              //A bitwise OR compares the corrseponding bits from two values, and if either bit is 1, 
                              //then the same bit in the result is also 1. Otherwise, it is 0.
                              regs[currentInstruction.getX()].setValue(regs[currentInstruction.getX()].getValue() | regs[currentInstruction.getY()].getValue());
                              pc += 2;
                              break;
                         case Instruction::TYPE_SHL: //17
                              //Set Vx = Vx SHL 1.
                              //If the most - significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
                              regs[0xF].setValue((regs[currentInstruction.getX()].getValue() & 0x80) >> 7);
                              regs[currentInstruction.getX()].setValue(regs[currentInstruction.getX()].getValue() << 1);
                              pc += 2;
                              break;


                         case Instruction::TYPE_SCRIGHT: //1 SUPER
                         case Instruction::TYPE_SCLEFT: //2 SUPER
                         case Instruction::TYPE_LOW: //3 SUPER
                         case Instruction::TYPE_HIGH: //4 SUPER
                         case Instruction::TYPE_SCDOWN: //7 SUPER
                         case Instruction::TYPE_ERROR: //0
                         case Instruction::TYPE_EOF: //28
#ifndef SUPPRESS_ALL_OUTPUT
                              cout << "Unhandled instruction type: " << currentInstruction.getType() << hex << " instr " << currentInstruction.getFullInstruction() << " at " << pc << endl;
#endif
                              exit(2);
               }

               cycleSpecificTasks();

          }
     }
#endif
     void cycleSpecificTasks();
#ifdef WONT_BE
     {
          //Do cycle specific tasks.
          //               view.drawToViewport();
          
          if (delayTimer.getValue() > 0)
          {
               delayTimer.setValue(delayTimer.getValue() - 1);
          }
          if (soundTimer.getValue() > 0)
          {
               soundTimer.setValue(soundTimer.getValue() - 1);
               cout << "BUZZ" << endl;
          }
     }
#endif
     VirtualMachine();
     ~VirtualMachine();
};

