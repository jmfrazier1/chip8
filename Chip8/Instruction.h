#include "stdafx.h"
#pragma once
using namespace std;

class Instruction
{
     /*
     //Instruction Set
     NNN is an address,
     KK is an 8 bit constant
     X and Y are two 4 bits constants

     0NNN	     Call 1802 machine code program at NNN (not implemented)
     00CN		Scroll down N lines (***)
     00FB		Scroll 4 pixels right (***)
     00FC		Scroll 4 pixels left (***)
     00FD		Quit the emulator (***)
     00FE		Set CHIP-8 graphic mode (***)
     00FF		Set SCHIP graphic mode (***)
     00E0		Erase the screen
     00EE		Return from a CHIP-8 sub-routine
     1NNN	     Jump to NNN
     2NNN	     Call CHIP-8 sub-routine at NNN (16 successive calls max)
     3XKK	     Skip next instruction if VX == KK
     4XKK	     Skip next instruction if VX != KK
     5XY0		Skip next instruction if VX == VY
     6XKK	     VX = KK
     7XKK	     VX = VX + KK
     8XY0		VX = VY
     8XY1		VX = VX OR VY
     8XY2		VX = VX AND VY
     8XY3		VX = VX XOR VY (*)
     8XY4		VX = VX + VY, VF = carry
     8XY5		VX = VX - VY, VF = not borrow (**)
     8XY6		VX = VX SHR 1 (VX=VX/2), VF = carry
     8XY7		VX = VY - VX, VF = not borrow (*) (**)
     8XYE		VX = VX SHL 1 (VX=VX*2), VF = carry
     9XY0		Skip next instruction if VX != VY
     ANNN	     I = NNN
     BNNN	     Jump to NNN + V0
     CXKK	     VX = Random number AND KK
     DXYN	     Draws a sprite at (VX,VY) starting at M(I). VF = collision. If N=0, draws the 16 x 16 sprite, else an 8 x N sprite.
     EX9E		Skip next instruction if key VX pressed
     EXA1		Skip next instruction if key VX not pressed
     FX07		VX = Delay timer
     FX0A		Waits a keypress and stores it in VX
     FX15		Delay timer = VX
     FX18		Sound timer = VX
     FX1E		I = I + VX
     FX29		I points to the 4 x 5 font sprite of hex char in VX
     FX33		Store BCD representation of VX in M(I)...M(I+2)
     FX55		Save V0...VX in memory starting at M(I)
     FX65		Load V0...VX from memory starting at M(I)
     FX75		Save V0...VX (X<8) in the HP48 flags (***)
     FX85		Load V0...VX (X<8) from the HP48 flags (***)

     (*): Used to be undocumented (but functional) in the original docs.

     (**): When you do VX - VY, VF is set to the negation of the borrow. This means that if VX is superior or equal to VY, VF will be set to 01, as the borrow is 0. If VX is inferior to VY, VF is set to 00, as the borrow is 1.

     (***):	SCHIP Instruction. Can be used in CHIP8 graphic mode.

     */
private:
     static unsigned int firstDataPointer;
     

     short type;
     unsigned int memoryOffset;
     unsigned int fullInstruction;
     short high8;
     short x;
     short y;
     short n;
     short kk;
     short nnn;
     string mnemonic;
     string label;

     //Decodes an instruction
     void decode();

public:
     static const short TYPE_ERROR = 0;
     static const short TYPE_SCRIGHT = 1;
     static const short TYPE_SCLEFT = 2;
     static const short TYPE_LOW = 3;
     static const short TYPE_HIGH = 4;
     static const short TYPE_CLS = 5;
     static const short TYPE_RTS = 6;
     static const short TYPE_SCDOWN = 7;
     static const short TYPE_JMP = 8;
     static const short TYPE_JSR = 9;
     static const short TYPE_SKEQ = 0xA;
     static const short TYPE_SKNE = 0xB;
     static const short TYPE_SKEQ_REG = 0xC;
     static const short TYPE_MOV = 0xD;
     static const short TYPE_ADD = 0xE;
     static const short TYPE_MOV_REG = 0xF;
     static const short TYPE_OR = 0x10;
     static const short TYPE_AND = 0x11;
     static const short TYPE_XOR = 0x12;
     static const short TYPE_ADD_REG = 0x13;
     static const short TYPE_SUB = 0x14;
     static const short TYPE_SHR = 0x15;
     static const short TYPE_RSB = 0x16;
     static const short TYPE_SHL = 0x17;
     static const short TYPE_SKNE_REG = 0x18;
     static const short TYPE_MVI = 0x19;
     static const short TYPE_JMI = 0x1A;
     static const short TYPE_RAND = 0x1B;
     static const short TYPE_SPRITE = 0x1C;
     static const short TYPE_SKPR = 0x1D;
     static const short TYPE_SKUP = 0x1E;
     static const short TYPE_GDELAY = 0x1F;
     static const short TYPE_KEY = 0x20;
     static const short TYPE_SDELAY = 0x21;
     static const short TYPE_GSOUND = 0x22;
     static const short TYPE_ADI = 0x23;
     static const short TYPE_FONT = 0x24;
     static const short TYPE_BCD = 0x25;
     static const short TYPE_STR = 0x26;
     static const short TYPE_LDR = 0x27;
     static const short TYPE_EOF = 0x28;

     static string itostr(int w, size_t hex_len);
     Instruction();
     Instruction(unsigned int offset, unsigned int instruction);
     ~Instruction();
     static Instruction readNext(FILE *rom);
     string getMnemonic() { return mnemonic; }
     bool isValid() { return (type != TYPE_ERROR && type != TYPE_EOF); };
     short getType() { return type; }
     short getNNN() { return nnn; }
     unsigned int getMemoryAddress() { return memoryOffset; }
     void setLabel(string instructionLabel) { label = instructionLabel; }

     Instruction(const Instruction &);
     Instruction &operator=(const Instruction &rhs);
     int operator==(const Instruction &rhs) const;
     int operator<(const Instruction &rhs) const;
     bool isDataOperation()
     {
          bool dataOp = false;
          switch (type)
          {
               case Instruction::TYPE_MVI:
                    dataOp = true;
                    break;
          }
          return dataOp;
     }
     bool isLabelOperation()
     {
          bool labelOp = false;
          switch (type)
          {
          case Instruction::TYPE_JSR:
          case Instruction::TYPE_JMP:
               labelOp = true;
               break;
          }
          return labelOp;
     }

};

