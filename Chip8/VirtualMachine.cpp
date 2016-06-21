#include "stdafx.h"
//#include "VirtualMachine.h"


VirtualMachine::VirtualMachine()
{
     //Load interpreter information
     loadInterpreterSpace();
     
     //Load the ROM
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/15PUZZLE.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/AIRPLANE.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/BLINKY.ch8");
     loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/BLITZ.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/BREAKOUT.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/BRIX.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/CAVE.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/CONNECT4.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/FIGURES.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/FILTER.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/GUESS.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/HIDDEN.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/INVADERS.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/KALEID.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/LANDING.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/MAZE.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/MERLIN.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/MISSILE.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/PADDLES.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/PONG(1P).ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/PONG.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/PONG2.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/PUZZLE.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/ROCKET.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/SOCCER.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/SPACEF.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/SQUASH.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/SYZYGY.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/TANK.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/TETRIS.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/TICTAC.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/TRON.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/UFO.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/VBRIX.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/VERS.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/WALL.ch8");
     //loadROM("C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/WIPEOFF.ch8");

     fopen_s(&KeyPressOutputFile, "keypressFile.txt", "w");
     fopen_s(&InstructionOutputFile, "Instruction.txt", "w");
     run();
}


VirtualMachine::~VirtualMachine()
{
     if (KeyPressOutputFile != NULL)
     {
          fclose(KeyPressOutputFile);
     }
     if (InstructionOutputFile != NULL)
     {
          fclose(InstructionOutputFile);
     }
}


void VirtualMachine::loadROM(string romName)
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


void VirtualMachine::loadInterpreterSpace()
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

int VirtualMachine::isKeyPressed(short key)
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
int VirtualMachine::isKeyUp(short key)
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
//This needs to work on the key press without needing to have enter pressed.
//Also only respond to the requisite key presses.
int VirtualMachine::waitForKeyPress()
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

unsigned short VirtualMachine::getRandomByte()
{
     unsigned short ret = 0;
     //generates a random number from 0 to 255
     int r = rand();
     ret = r % 255;
     return ret;
}


void VirtualMachine::run()
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

void VirtualMachine::cycleSpecificTasks()
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
