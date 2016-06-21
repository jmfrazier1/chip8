// Chip8.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include "Instruction.h"
//#include <list>
//#include <vector>
//#include <algorithm>
//#include "VirtualMachine.h"
using namespace std;


void xorTest()
{
     /*

     Collision detection

     Original 0000 0000 new anything, no collision
     Original 0000 0001 new 0000 0000 Collision
     not
     1111 1110 XOR 0000 0000
     Original 0000 0001 new 1111 1111 No Collision




     */
     unsigned char u1;
     unsigned char u2;
     unsigned char res;

     int isCollision = 0;

     //Not a collision, res = ff
     u1 = 0x00;// 0000 0000
     u2 = 0x00;// 0000 0000
     res = (~u1) ^ (~u2);
     isCollision = ViewPort::isCollision(u1, u2);
     cout << "Comparing " << hex << (int)u1 << " to " << (int)u2 << " is " << isCollision << "" << endl;



     //Is a collision, res = fe
     u1 = 0x01;// 0000 0001
     u2 = 0x00;// 0000 0000
     res = (~u1) ^ (~u2);
     isCollision = ViewPort::isCollision(u1, u2);
     cout << "Comparing " << hex << (int)u1 << " to " << (int)u2 << " is " << isCollision << "" << endl;

     //Not a collision, res = 1
     u1 = 0x01;// 0000 0001
     u2 = 0xff;// 1111 1111

     res = (~u1)^(~u2);
     isCollision = ViewPort::isCollision(u1, u2);
     cout << "Comparing " << hex << (int)u1 << " to " << (int)u2 << " is " << isCollision << "" << endl;

}

int _tmain(int argc, _TCHAR* argv[])
{
     
     //xorTest();
          
     VirtualMachine vm;
}

int _tmain1(int argc, _TCHAR* argv[])
{

     FILE *romFile;
     errno_t retVal = fopen_s(&romFile, "C:/Users/Jeff/Desktop/Chip8/Sample ROMs/GAMES/15PUZZLE.ch8", "rb");

     if (retVal != 0)
     {
          cout << "Failed to open the file: "  << retVal << endl;
               return 1;
     }
     cout << "Opened the file properly." << endl;

     //list<Instruction> instructionList;
     vector<Instruction> instructionVector;


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
               << ins.getFullInstruction()
               << "\t"
               << ((ins.getLabel().length() > 0) ? ins.getLabel() + "\t" : "\t\t")
               << ins.getMnemonic()
//               << " For " << &ins
               << endl;

     }




     //instructionList.begin();
     //for (std::list<Instruction>::iterator it = instructionList.begin(); it != instructionList.end(); it++)
     //{
     //     Instruction ins = *it;
     //     string myMnemonic = ins.getMnemonic();
     //     //cout <<  << ' ';

     //}

// 
     /*
     Instruction myInstr = Instruction::readNext(romFile);
     instructionList.insert(instructionList.end(), myInstr);

     Instruction myInstr2 = Instruction::readNext(romFile);
     instructionList.insert(instructionList.end(), myInstr2);

     Instruction myInstr3 = Instruction::readNext(romFile);

     Instruction myInstr4 = Instruction::readNext(romFile);
     
     Instruction myInstr5 = Instruction::readNext(romFile);

     Instruction myInstr6 = Instruction::readNext(romFile);
     Instruction myInstr7 = Instruction::readNext(romFile);
     Instruction myInstr8 = Instruction::readNext(romFile);
     Instruction myInstr9 = Instruction::readNext(romFile);
     Instruction myInstr10 = Instruction::readNext(romFile);
     Instruction myInstr11 = Instruction::readNext(romFile);
     Instruction myInstr12 = Instruction::readNext(romFile);
     Instruction myInstr13 = Instruction::readNext(romFile);
     Instruction myInstr14 = Instruction::readNext(romFile);
     Instruction myInstr15 = Instruction::readNext(romFile);
     Instruction myInstr16 = Instruction::readNext(romFile);
     Instruction myInstr17 = Instruction::readNext(romFile);
     Instruction myInstr18 = Instruction::readNext(romFile);
     Instruction myInstr19 = Instruction::readNext(romFile);
     Instruction myInstr20 = Instruction::readNext(romFile);
     Instruction myInstr21 = Instruction::readNext(romFile);
     Instruction myInstr22 = Instruction::readNext(romFile);
     Instruction myInstr23 = Instruction::readNext(romFile);
     Instruction myInstr24 = Instruction::readNext(romFile);
     Instruction myInstr25 = Instruction::readNext(romFile);
     Instruction myInstr26 = Instruction::readNext(romFile);
     Instruction myInstr27 = Instruction::readNext(romFile);
     Instruction myInstr28 = Instruction::readNext(romFile);
     Instruction myInstr29 = Instruction::readNext(romFile);
     Instruction myInstr30 = Instruction::readNext(romFile);
     Instruction myInstr31 = Instruction::readNext(romFile);
     Instruction myInstr32 = Instruction::readNext(romFile);
     Instruction myInstr33 = Instruction::readNext(romFile);
     Instruction myInstr34 = Instruction::readNext(romFile);
     Instruction myInstr35 = Instruction::readNext(romFile);
     Instruction myInstr36 = Instruction::readNext(romFile);
     Instruction myInstr37 = Instruction::readNext(romFile);
     Instruction myInstr38 = Instruction::readNext(romFile);


     Instruction myInstr39 = Instruction::readNext(romFile);
     Instruction myInstr40 = Instruction::readNext(romFile);
     Instruction myInstr41 = Instruction::readNext(romFile);
     Instruction myInstr42 = Instruction::readNext(romFile);
     Instruction myInstr43 = Instruction::readNext(romFile);
     Instruction myInstr44 = Instruction::readNext(romFile);

     Instruction myInstr45 = Instruction::readNext(romFile);
     Instruction myInstr46 = Instruction::readNext(romFile);
     Instruction myInstr47 = Instruction::readNext(romFile);
     Instruction myInstr48 = Instruction::readNext(romFile);
     Instruction myInstr49 = Instruction::readNext(romFile);
     Instruction myInstr50 = Instruction::readNext(romFile);
     Instruction myInstr51 = Instruction::readNext(romFile);

     Instruction myInstr52 = Instruction::readNext(romFile);
     Instruction myInstr53 = Instruction::readNext(romFile);
     Instruction myInstr54 = Instruction::readNext(romFile);
     Instruction myInstr55 = Instruction::readNext(romFile);
     Instruction myInstr56 = Instruction::readNext(romFile);
     Instruction myInstr57 = Instruction::readNext(romFile);
     Instruction myInstr58 = Instruction::readNext(romFile);
     Instruction myInstr59 = Instruction::readNext(romFile);
     Instruction myInstr60 = Instruction::readNext(romFile);

     Instruction myInstr61 = Instruction::readNext(romFile);
     Instruction myInstr62 = Instruction::readNext(romFile);
     Instruction myInstr63 = Instruction::readNext(romFile);
     Instruction myInstr64 = Instruction::readNext(romFile);
     Instruction myInstr65 = Instruction::readNext(romFile);
     Instruction myInstr66 = Instruction::readNext(romFile);
     Instruction myInstr67 = Instruction::readNext(romFile);
     Instruction myInstr68 = Instruction::readNext(romFile);
     Instruction myInstr69 = Instruction::readNext(romFile);
     Instruction myInstr70 = Instruction::readNext(romFile);

     Instruction myInstr71 = Instruction::readNext(romFile);
     Instruction myInstr72 = Instruction::readNext(romFile);
     Instruction myInstr73 = Instruction::readNext(romFile);
     Instruction myInstr74 = Instruction::readNext(romFile);
     Instruction myInstr75 = Instruction::readNext(romFile);
     Instruction myInstr76 = Instruction::readNext(romFile);
     Instruction myInstr77 = Instruction::readNext(romFile);
     Instruction myInstr78 = Instruction::readNext(romFile);
     Instruction myInstr79 = Instruction::readNext(romFile);
     Instruction myInstr80 = Instruction::readNext(romFile);

     Instruction myInstr81 = Instruction::readNext(romFile);
     Instruction myInstr82 = Instruction::readNext(romFile);
     Instruction myInstr83 = Instruction::readNext(romFile);
     Instruction myInstr84 = Instruction::readNext(romFile);
     Instruction myInstr85 = Instruction::readNext(romFile);
     Instruction myInstr86 = Instruction::readNext(romFile);
     Instruction myInstr87 = Instruction::readNext(romFile);
     Instruction myInstr88 = Instruction::readNext(romFile);
     Instruction myInstr89 = Instruction::readNext(romFile);
     Instruction myInstr90 = Instruction::readNext(romFile);

     Instruction myInstr91 = Instruction::readNext(romFile);
     Instruction myInstr92 = Instruction::readNext(romFile);
     Instruction myInstr93 = Instruction::readNext(romFile);
     Instruction myInstr94 = Instruction::readNext(romFile);
     Instruction myInstr95 = Instruction::readNext(romFile);
     Instruction myInstr96 = Instruction::readNext(romFile);
     Instruction myInstr97 = Instruction::readNext(romFile);
     Instruction myInstr98 = Instruction::readNext(romFile);
     Instruction myInstr99 = Instruction::readNext(romFile);
     Instruction myInstr100 = Instruction::readNext(romFile);

     Instruction myInstr101 = Instruction::readNext(romFile);
     Instruction myInstr102 = Instruction::readNext(romFile);
     Instruction myInstr103 = Instruction::readNext(romFile);
     Instruction myInstr104 = Instruction::readNext(romFile);
     Instruction myInstr105 = Instruction::readNext(romFile);
     Instruction myInstr106 = Instruction::readNext(romFile);
     Instruction myInstr107 = Instruction::readNext(romFile);
     Instruction myInstr108 = Instruction::readNext(romFile);
     Instruction myInstr109 = Instruction::readNext(romFile);
     Instruction myInstr110 = Instruction::readNext(romFile);

     Instruction myInstr111 = Instruction::readNext(romFile);
     Instruction myInstr112 = Instruction::readNext(romFile);
     Instruction myInstr113 = Instruction::readNext(romFile);
     Instruction myInstr114 = Instruction::readNext(romFile);
     Instruction myInstr115 = Instruction::readNext(romFile);
     Instruction myInstr116 = Instruction::readNext(romFile);
     
     
     Instruction myInstr117 = Instruction::readNext(romFile);
     Instruction myInstr118 = Instruction::readNext(romFile);
     Instruction myInstr119 = Instruction::readNext(romFile);
     Instruction myInstr120 = Instruction::readNext(romFile);

     Instruction myInstr121 = Instruction::readNext(romFile);
     Instruction myInstr122 = Instruction::readNext(romFile);
     Instruction myInstr123 = Instruction::readNext(romFile);
     Instruction myInstr124 = Instruction::readNext(romFile);
     Instruction myInstr125 = Instruction::readNext(romFile);
     Instruction myInstr126 = Instruction::readNext(romFile);
     Instruction myInstr127 = Instruction::readNext(romFile);
     Instruction myInstr128 = Instruction::readNext(romFile);
     Instruction myInstr129 = Instruction::readNext(romFile);
     Instruction myInstr130 = Instruction::readNext(romFile);

     Instruction myInstr131 = Instruction::readNext(romFile);
     Instruction myInstr132 = Instruction::readNext(romFile);
     Instruction myInstr133 = Instruction::readNext(romFile);
     Instruction myInstr134 = Instruction::readNext(romFile);
     Instruction myInstr135 = Instruction::readNext(romFile);
     Instruction myInstr136 = Instruction::readNext(romFile);
     Instruction myInstr137 = Instruction::readNext(romFile);
     Instruction myInstr138 = Instruction::readNext(romFile);
     Instruction myInstr139 = Instruction::readNext(romFile);
     Instruction myInstr140 = Instruction::readNext(romFile);

     //Instruction myInstr11 = Instruction::readNext(romFile);
     //Instruction myInstr12 = Instruction::readNext(romFile);
     //Instruction myInstr13 = Instruction::readNext(romFile);
     //Instruction myInstr14 = Instruction::readNext(romFile);
     //Instruction myInstr15 = Instruction::readNext(romFile);
     //Instruction myInstr16 = Instruction::readNext(romFile);
     //Instruction myInstr17 = Instruction::readNext(romFile);
     //Instruction myInstr18 = Instruction::readNext(romFile);
     //Instruction myInstr19 = Instruction::readNext(romFile);
     //Instruction myInstr10 = Instruction::readNext(romFile);

     // */


     fclose(romFile);

     system("pause");
	return 0;
}

