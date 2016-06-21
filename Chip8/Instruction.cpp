#include "stdafx.h"
//#include "Instruction.h"
using namespace std;

//#define EMIT_COMMENTS               

Instruction::Instruction()
{
     label = "";
}

string Instruction::itostr(int w, size_t hex_len = sizeof(int) << 1) {
     static const char* digits = "0123456789ABCDEF";
     string rc(hex_len, '0');
     for (size_t i = 0, j = (hex_len - 1) * 4; i<hex_len; ++i, j -= 4)
          rc[i] = digits[(w >> j) & 0x0f];
     return rc;
}
Instruction::Instruction(unsigned int offset, unsigned int instruction)
{
     label = "";
     memoryOffset = offset;
     fullInstruction = instruction;
     decode();
}

Instruction::~Instruction()
{
}



void Instruction::decode()
{
     //Compute the possible needed values
     high8 = (fullInstruction & 0xF000) >> 12;
     x =     (fullInstruction & 0x0F00) >> 8;
     y =     (fullInstruction & 0x00F0) >> 4;
     n =     (fullInstruction & 0x000F);
     kk =    (fullInstruction & 0x00FF);
     nnn =   (fullInstruction & 0x0FFF);
     
     //stringstream stream;
     //stream << std::hex << your_int;
     //std::string result(stream.str());
     
     //string mystring;
     switch (high8)
     {
     case 0x0:
          //Graphics control
          //Upper first byte should also be 0x0.
          if (x == 0)
          {
               switch (kk)
               {
               case 0xE0:
                    type = TYPE_CLS;
                    mnemonic = "CLS";
                    break;
               case 0xEE:
                    type = TYPE_RTS;
                    mnemonic = "RTS";
                    break;
               default:
                    type = TYPE_ERROR;
                    mnemonic = "Invalid instruction found " + itostr(fullInstruction, 4);
                    break;
               }
          }
          else
          {
               //This is a jump that is not implemented
               type = TYPE_ERROR;
               mnemonic = "Invalid instruction found " + itostr(fullInstruction, 4);
          }
          break;
     case 0x1:
          //Jump instruction
          //1NNN	     Jump to NNN
          type = TYPE_JMP;
          mnemonic = "JMP LABEL_" + itostr(nnn, 4);
          //cout << "Mnemonic is " << mnemonic << endl;
          break;
     case 0x2:
          //Call subroutine
          //2NNN	     Call CHIP-8 sub-routine at NNN (16 successive calls max)
          type = TYPE_JSR;
          mnemonic = "JSR LABEL_" + itostr(nnn, 4);
          break;
     case 0x3:
          //Skip next if VX == KK
          type = TYPE_SKEQ;
          mnemonic = "SKEQ V" + itostr(x, 1) + ", " + itostr(kk, 2);
          //cout << "mnemonic " << mnemonic << endl;
          break;
     case 0x4:
          //Skip next if VX != KK
          type = TYPE_SKNE;
          mnemonic = "SKNE V" + itostr(x, 1) + ", " + itostr(kk, 2);
          break;
     case 0x5:
          //Skip next if VX == VY
          type = TYPE_SKEQ_REG;
          mnemonic = "SKEQ V" + itostr(x, 1) + ", V" + itostr(y, 1);
          break;
     case 0x6:
          //VX = KK
          //Set the value of the VX register to the 16 bit constant
          type = TYPE_MOV;
          mnemonic = "MOV V" + itostr(x, 1) + ", " + itostr(kk, 2);
          break;
     case 0x7:
          //VX = VX + KK
          //7XKK	VX = VX + KK
          type = TYPE_ADD;
          mnemonic = "ADD V" + itostr(x, 1) + ", " + itostr(kk, 2);
          break;
     case 0x8:
          //
          switch (n)
          {
          case 0x0:
               //8XY0		VX = VY
               type = TYPE_MOV_REG;
               mnemonic = "MOV V" + itostr(x, 1) + "," + itostr(y, 1);
               break;
          case 0x1:
               //8XY1		VX = VX OR VY
               type = TYPE_OR;
               mnemonic = "OR V" + itostr(x, 1) + ", V" + itostr(y, 1);
               break;
          case 0x2:
               //8XY2		VX = VX AND VY
               type = TYPE_AND;
               mnemonic = "AND V" + itostr(x, 1) + ", V" + itostr(y, 1);
               break;
          case 0x3:
               //8XY3		VX = VX XOR VY(*)
               type = TYPE_XOR;
               mnemonic = "XOR V" + itostr(x, 1) + ", V" + itostr(y, 1);
               break;
          case 0x4:
               //8XY4		VX = VX + VY, VF = carry
               type = TYPE_ADD_REG;
               mnemonic = "ADD V" + itostr(x, 1) + ", V" + itostr(y, 1);
               break;
          case 0x5:
               //8XY5		VX = VX - VY, VF = not borrow(**)
               type = TYPE_SUB;
               mnemonic = "SUB V" + itostr(x, 1) + ", V" + itostr(y, 1);
               break;
          case 0x6:
               //8XY6		VX = VX SHR 1 (VX = VX / 2), VF = carry
               type = TYPE_SHR;
               mnemonic = "SHR V" + itostr(x, 1) + ", V" + itostr(y, 1);
               break;
          case 0x7:
               //8XY7		VX = VY - VX, VF = not borrow(*) (**)
               type = TYPE_RSB;
               mnemonic = "RSB V" + itostr(x, 1) + ", V" + itostr(y, 1);
               break;
          case 0xE:
               //8XYE		VX = VX SHL 1 (VX = VX * 2), VF = carry
               type = TYPE_SHL;
               mnemonic = "SHL V" + itostr(x, 1) + ", V" + itostr(y, 1);
               break;
          default:
               type = TYPE_ERROR;
               mnemonic = "Invalid instruction found " + itostr(fullInstruction, 4);
               break;
          }
          break;
     case 0x9:
          //Skip next if VX != VY
          type = TYPE_SKNE_REG;
          mnemonic = "SKNE V" + itostr(x, 1) + ", V" + itostr(y, 1);
          break;
     case 0xA:
          //I = NNN
          type = TYPE_MVI;
          mnemonic = "MVI " + itostr(nnn, 3);
          break;
     case 0xB:
          //Jump to NNN + V0
          type = TYPE_JMI;
          mnemonic = "JMI " + itostr(nnn, 3);
          break;
     case 0xC:
          //VX = Random number AND KK
          type = TYPE_RAND;
          mnemonic = "RAND V" + itostr(x, 1) + "," + itostr(kk, 2);
          break;
     case 0xD:
          //Draws a sprite at (VX,VY) starting at M(I). VF = collision. If N=0, draws the 16 x 16 sprite, else an 8 x N sprite.
          //DXYN	     Draws a sprite at(VX, VY) starting at M(I).VF = collision.If N = 0, draws the 16 x 16 sprite, else an 8 x N sprite.
          type = TYPE_SPRITE;
          mnemonic = "SPRITE V" + itostr(x, 1) + "," + itostr(y, 1) + "," + itostr(n, 1);
          break;
     case 0xE:
          //Keypress control
          switch (kk)
          {
          case 0x9E:
               //EX9E		Skip next instruction if key VX pressed
               type = TYPE_SKPR;
               mnemonic = "SKPR V" + itostr(x, 1);
               break;
          case 0xA1:
               //EXA1		Skip next instruction if key VX not pressed
               type = TYPE_SKUP;
               mnemonic = "SKUP V" + itostr(x, 1);
               break;
          default:
               type = TYPE_ERROR;
               mnemonic = "Invalid instruction found " + itostr(fullInstruction, 4);
               break;
          }
          break;
     case 0xF:
          //load control
          switch (kk)
          {
          case 0x07:
               //fr07	gdelay vr	get delay timer into vr
               type = TYPE_GDELAY;
               mnemonic = "GDELAY V" + itostr(x, 1);
               break;
          case 0x0A:
               //fr0a	key vr	wait for for keypress,put key in register vr
               type = TYPE_KEY;
               mnemonic = "KEY V" + itostr(x, 1);
               break;
          case 0x15:
               //fr15	sdelay vr	set the delay timer to vr
               type = TYPE_SDELAY;
               mnemonic = "SDELAY V" + itostr(x, 1);
               break;
          case 0x18:
               //fr18	ssound vr	set the sound timer to vr
               type = TYPE_GSOUND;
               mnemonic = "GSOUND V" + itostr(x, 1);
               break;
          case 0x1E:
               //I = I + VX
               type = TYPE_ADI;
               mnemonic = "ADI V" + itostr(x, 1);
               break;
          case 0x29:
               //FX29
               //I points to the 4 x 5 font sprite of hex char in VX
               type = TYPE_FONT;
               mnemonic = "FONT V" + itostr(x, 1);
               break;
          case 0x33:
               //FX29
               //I points to the 4 x 5 font sprite of hex char in VX
               type = TYPE_BCD;
               mnemonic = "BCD V" + itostr(x, 1);
               break;
          case 0x55:
               //Save V0...VX in memory starting at M(I)
               //LD[I], V0
               type = TYPE_STR;
               mnemonic = "STR V0-V" + itostr(x, 1);
               break;
          case 0x65:
               //Load V0...VX from memory starting at M(I)
               type = TYPE_LDR;
               mnemonic = "LDR V0-V" + itostr(x, 1);
               break;
          default:
               type = TYPE_ERROR;
               mnemonic = "Invalid instruction found " + itostr(fullInstruction, 4);
               break;
          }
          break;

     }
     //cout << dec;
     //cout << endl;


}

Instruction Instruction::readNext(FILE *rom)
{
     //Each instruction is 2 bytes long
     Instruction ins;
     int instr1;
     int instr2;

     //Get file position
     fpos_t pos;
     int posret = fgetpos(rom, &pos);

     if (posret != 0)
     {
          cout << "Failed to get file position " << posret << endl;
          ins.type = TYPE_EOF;
          return ins;
     }

     //Memory offset
     pos += 0x200;

     //cout << hex;
     //cout << "0X" << pos << "\t";

     ins.memoryOffset = (unsigned int)pos;

     //Validate the read
     instr1 = getc(rom);
     if (instr1 == EOF)
     {
          ins.type = TYPE_EOF;
          return ins;
     }
     //cout << "The read byte is ";
     //printf("%02x", instr1);
     //cout << endl;

     instr2 = getc(rom);
     if (instr2 == EOF)
     {
          ins.type = TYPE_EOF;
          return ins;
     }
     //cout << "The read byte is ";
     //printf("%02x", instr2);
     //cout << endl;

     //The bit shifting may need to be changed when there is a different architecture
     //unsigned int fullInstruction = (instr1 << 8) | instr2;
     
     //cout << "Full instruction is ";
     //printf("%04x", fullInstruction);
     //cout << endl;

     ins.fullInstruction = (instr1 << 8) | instr2;
     ins.decode();

     //ins.getMnemonic();

/*

     //Get only the upper 
     unsigned short upperFirstByte = (fullInstruction & 0xF000) >> 12;
     short x;
     short y;
     short n;
     short kk;
     short nnn;
     //cout << "upperFirstByte: " << upperFirstByte << endl;
     switch (upperFirstByte)
     {
          case 0x0:
               //Graphics control
               //Upper first byte should also be 0x0.
               if ((fullInstruction & 0x0F00) == 0)
               {
                    switch (fullInstruction & 0x00FF)
                    {
                         case 0xFB:
                              cout << "EMIT:   SCRIGHT";
#ifdef EMIT_COMMENTS               
                              cout << " ; Scroll 4 pixels right";
#endif
                              cout << endl;
                              //cout << "Scroll 4 pixels right" << endl;
                              break;
                         case 0xFC:
                              cout << "EMIT:   SCLEFT";
#ifdef EMIT_COMMENTS               
                              cout << " ; Scroll 4 pixels left";
#endif
                              cout << endl;
                              //cout << "Scroll 4 pixels left" << endl;
                              break;
                         case 0xFD:
                              cout << "Quit the emulator" << endl;
                              break;
                         case 0xFE:
                              cout << "EMIT:   LOW";
#ifdef EMIT_COMMENTS               
                              cout << " ; Set CHIP-8 graphic mode";
#endif
                              cout << endl;
                              //cout << "Set CHIP-8 graphic mode" << endl;
                              break;
                         case 0xFF:
                              cout << "EMIT:   HIGH";
#ifdef EMIT_COMMENTS               
                              cout << " ; Set SCHIP graphic mode";
#endif
                              cout << endl;
                              //cout << "Set SCHIP graphic mode" << endl;
                              break;
                         case 0xE0:
                              //cout << "Erase the screen" << endl;
                              cout << "EMIT:   CLS";
#ifdef EMIT_COMMENTS               
                              cout << " ; Equates to Erase the screen";
#endif
                              cout << endl;
                              break;
                         case 0xEE:
                              cout << "EMIT:   RTS";
#ifdef EMIT_COMMENTS               
                              cout << " ; Return from a CHIP-8 sub-routine";
#endif
                              cout << endl;
                              //cout << "Return from a CHIP-8 sub-routine" << endl;
                              break;
                         default:
                              if (((fullInstruction & 0x00F0) >> 4) == 0xC)
                              {
                                   //This is a scroll down N lines command.
                                   n = (fullInstruction & 0x000F);
                                   cout << "EMIT:   SCDOWN " << n;
#ifdef EMIT_COMMENTS               
                                   cout << " ; Scroll down " << n << " lines";
#endif
                                   cout << endl;
                                   //cout << "Scroll down N lines where N = 0x" << scrollLines << endl;
                              }
                              else
                              {
                                   cout << "Invlaid instruction found " << fullInstruction << endl;
                              }
                              break;
                    }
               }
               else
               {
                    //This is a jump that is not implemented
                    cout << "1802 instruction that will not be handled " << fullInstruction << endl;
               }
               break;
          case 0x1:
               //Jump instruction
               //1NNN	     Jump to NNN
               nnn = (fullInstruction & 0x0FFF);
               cout << "EMIT: JMP " << nnn;
#ifdef EMIT_COMMENTS               
               cout << " ; JP " << nnn << " Might not be fully implemented.";
#endif
               cout << endl;
               //cout << "Not handled yet " << fullInstruction << dec << endl;
               break;
          case 0x2:
               //Call subroutine
               //2NNN	     Call CHIP-8 sub-routine at NNN (16 successive calls max)
               nnn = (fullInstruction & 0x0FFF);
               cout << "EMIT: JSR " << nnn;
#ifdef EMIT_COMMENTS               
               cout << " ; CALL " << nnn << " Might not be fully implemented.";
#endif
               cout << endl;
               //cout << "Not handled yet " << fullInstruction << dec << endl;
               break;
          case 0x3:
               //Skip next if VX == KK
               x = (fullInstruction & 0x0F00) >> 8;
               kk = (fullInstruction & 0x00FF);
               cout << "EMIT: SKEQ V" << x << "," << kk;
#ifdef EMIT_COMMENTS               
               cout << " ; ? " << x << kk << " ?.";
#endif
               cout << endl;
               //cout << "Not handled yet " << fullInstruction << dec << endl;
               break;
          case 0x4:
               //Skip next if VX != KK
               x = (fullInstruction & 0x0F00) >> 8;
               kk = (fullInstruction & 0x00FF);
               cout << "EMIT: SKNE V" << x << ", " << kk;
#ifdef EMIT_COMMENTS               
               cout << " ; Skip next if V" << x << " != " << kk;
#endif
               cout << endl;
               //cout << "Not handled yet " << fullInstruction << dec << endl;
               break;
          case 0x5:
               //Skip next if VX == VY
               x = (fullInstruction & 0x0F00) >> 8;
               y = (fullInstruction & 0x00F0) >> 4;
               cout << "EMIT: SKEQ V" << x << ", V" << y;
#ifdef EMIT_COMMENTS               
               cout << " ; Skip next if V" << x << " == V" << y;
#endif
               cout << endl;
               //cout << "Not handled yet " << fullInstruction << dec << endl;
               break;
          case 0x6:
               //VX = KK
               //Set the value of the VX register to the 16 bit constant
               x  = (fullInstruction & 0x0F00) >> 8;
               kk = (fullInstruction & 0x00FF);
               cout << "EMIT: MOV V" << x << ", " << kk;
#ifdef EMIT_COMMENTS               
               cout << " ; Equates to LOD V" << x << " = " << kk;
#endif
               cout << endl;
               //cout << "Not handled yet " << fullInstruction << dec << endl;
               break;
          case 0x7:
               //VX = VX + KK
               //7XKK	VX = VX + KK
               x = (fullInstruction & 0x0F00) >> 8;
               kk = (fullInstruction & 0x00FF);
               cout << "EMIT: ADD V" << x << ", " << kk;
#ifdef EMIT_COMMENTS               
               cout << " ; Equates to V" << x << " = V" << x << " + " << kk;
#endif
               cout << endl;
               //cout << "Not handled yet " << fullInstruction << dec << endl;
               break;
          case 0x8:
               //
               x = (fullInstruction & 0x0F00) >> 8;
               y = (fullInstruction & 0x00F0) >> 4;
               n = (fullInstruction & 0x000F);
               switch (n)
               {
                    case 0x0:
                         //8XY0		VX = VY
                         cout << "EMIT: MOV V" << x << ", V" << y;
#ifdef EMIT_COMMENTS               
                         cout << " ; Equates to ? " << x << " .";
#endif
                         cout << endl;
                         break;
                    case 0x1:
                         //8XY1		VX = VX OR VY
                         cout << "EMIT: OR V" << x << ", V" << y;
#ifdef EMIT_COMMENTS               
                         cout << " ; MATH V" << x << " = V" << x << " OR V" << y;
#endif
                         cout << endl;
                         break;
                    case 0x2:
                         //8XY2		VX = VX AND VY
                         cout << "EMIT: AND V" << x << ", V" << y;
#ifdef EMIT_COMMENTS               
                         cout << " ; MATH V" << x << " = V" << x << " AND V" << y;
#endif
                         cout << endl;
                         break;
                    case 0x3:
                         //8XY3		VX = VX XOR VY(*)
                         cout << "EMIT: XOR V" << x << ", V" << y;
#ifdef EMIT_COMMENTS               
                         cout << " ; MATH V" << x << " = V" << x << " XOR V" << y << "(*)";
#endif
                         cout << endl;
                         break;
                    case 0x4:
                         //8XY4		VX = VX + VY, VF = carry
                         cout << "EMIT: ADD V" << x << ", V" << y;
#ifdef EMIT_COMMENTS               
                         cout << " ; ADC V" << x << " = V" << x << " + V" << y << " Carry to VF";
#endif
                         cout << endl;
                         break;
                    case 0x5:
                         //8XY5		VX = VX - VY, VF = not borrow(**)
                         cout << "EMIT: SUB V" << x << ", V" << y;
#ifdef EMIT_COMMENTS               
                         cout << " ; MATH V" << x << " = V" << x << " + V" << y << " Carry to VF";
#endif
                         cout << endl;
                         break;
                    case 0x6:
                         //8XY6		VX = VX SHR 1 (VX = VX / 2), VF = carry
                         cout << "EMIT: SHR V" << x << ", V" << y;
#ifdef EMIT_COMMENTS               
                         cout << " ; SHR V" << x << " = V" << x << " OR V" << y;
#endif
                         cout << endl;
                         break;
                    case 0x7:
                         //8XY7		VX = VY - VX, VF = not borrow(*) (**)
                         cout << "EMIT: RSB V" << x << ", V" << y;
#ifdef EMIT_COMMENTS               
                         cout << " ; RSB V" << x << " = V" << x << " OR V" << y;
#endif
                         cout << endl;
                         break;
                    case 0xE:
                         //8XYE		VX = VX SHL 1 (VX = VX * 2), VF = carry
                         cout << "EMIT: SHL V" << x << ", V" << y;
#ifdef EMIT_COMMENTS               
                         cout << " ; SHL V" << x << " = V" << x << " OR V" << y;
#endif
                         cout << endl;
                         break;
                    default:
                         cout << "Not handled yet " << fullInstruction << dec << endl;
                         break;
               }
               //cout << "Not handled yet " << fullInstruction << dec << endl;
               break;
          case 0x9:
               //Skip next if VX != VY
               x = (fullInstruction & 0x0F00) >> 8;
               y = (fullInstruction & 0x00F0) >> 4;
               cout << "EMIT: SKNE V" << x << ", V" << y;
#ifdef EMIT_COMMENTS               
               cout << " ; SKNE V" << x << " = V" << x << " OR V" << y;
#endif
               cout << endl;
               //cout << "Not handled yet " << fullInstruction << dec << endl;
               break;
          case 0xA:
               //I = NNN
               nnn = (fullInstruction & 0x0FFF);
               cout << "EMIT: MVI " << nnn;
#ifdef EMIT_COMMENTS               
               cout << " ; Equates to I = " << nnn << " Might not be fully implemented.";
#endif
               cout << endl;

               //Any operation which uses I includes data
               if (nnn < ins.firstDataPointer)
               {
                    ins.firstDataPointer = nnn;
               }
               //cout << "Not handled yet " << fullInstruction << dec << endl;
               break;
          case 0xB:
               //Jump to NNN + V0
               nnn = (fullInstruction & 0x0FFF);
               cout << "EMIT: JMI " << nnn;
#ifdef EMIT_COMMENTS               
               cout << " ; Equates to I = " << nnn << " Might not be fully implemented.";
#endif
               cout << endl;
               //cout << "Not handled yet " << fullInstruction << dec << endl;
               break;
          case 0xC:
               //VX = Random number AND KK
               x = (fullInstruction & 0x0F00) >> 8;
               kk = (fullInstruction & 0x00FF);
               cout << "EMIT: RAND V" << x << "," << kk;
#ifdef EMIT_COMMENTS               
               cout << " ; Equates to I = " << nnn << " Might not be fully implemented.";
#endif
               cout << endl;
               //cout << "Not handled yet " << fullInstruction << dec << endl;
               break;
          case 0xD:
               //Draws a sprite at (VX,VY) starting at M(I). VF = collision. If N=0, draws the 16 x 16 sprite, else an 8 x N sprite.
               //DXYN	     Draws a sprite at(VX, VY) starting at M(I).VF = collision.If N = 0, draws the 16 x 16 sprite, else an 8 x N sprite.
               x = (fullInstruction & 0x0F00) >> 8;
               y = (fullInstruction & 0x00F0) >> 4;
               n = (fullInstruction & 0x000F);
               cout << "EMIT: SPRITE V" << x << ",V" << y << "," << n;
#ifdef EMIT_COMMENTS               
               cout << " ; Equates to DRW  " << x << " .";
#endif
               cout << endl;
               //cout << "Not handled yet " << fullInstruction << dec << endl;
               break;
          case 0xE:
               //Keypress control
               x = (fullInstruction & 0x0F00) >> 8;
               kk = (fullInstruction & 0x00FF);
               switch (kk)
               {
                    case 0x9E:
                         //EX9E		Skip next instruction if key VX pressed
                         cout << "EMIT: SKPR V" << x;
#ifdef EMIT_COMMENTS               
                         cout << " ; Equates to Skip next line if key " << x << " is pressed.";
#endif
                         cout << endl;
                         break;
                    case 0xA1:
                         //EXA1		Skip next instruction if key VX not pressed
                         cout << "EMIT: SKUP V" << x;
#ifdef EMIT_COMMENTS               
                         cout << " ; Equates to Skip next line if key " << x << " is NOT pressed.";
#endif
                         cout << endl;
                         break;
                    default:
                         cout << "Not handled yet or error " << fullInstruction << dec << endl;
                         break;
               }
               break;
          case 0xF:
               //load control
               x = (fullInstruction & 0x0F00) >> 8;
               kk = (fullInstruction & 0x00FF);
               switch (kk)
               {
                    case 0x07: 
                         //fr07	gdelay vr	get delay timer into vr
                         cout << "EMIT: GDELAY V" << x;
#ifdef EMIT_COMMENTS               
                         cout << " ; Equates to ? " << x << " .";
#endif
                         cout << endl;
                         //cout << "Not handled yet " << fullInstruction << dec << endl;
                         break;
                    case 0x0A:
                         //fr0a	key vr	wait for for keypress,put key in register vr
                         cout << "EMIT: KEY V" << x;
#ifdef EMIT_COMMENTS               
                         cout << " ; Equates to ? " << x << " .";
#endif
                         cout << endl;
                         //cout << "Not handled yet " << fullInstruction << dec << endl;
                         break;
                    case 0x15:
                         //fr15	sdelay vr	set the delay timer to vr
                         cout << "EMIT: SDELAY V" << x;
#ifdef EMIT_COMMENTS               
                         cout << " ; Equates to ? " << x << " .";
#endif
                         cout << endl;
                         //cout << "Not handled yet " << fullInstruction << dec << endl;
                         break;
                    case 0x18:
                         //fr18	ssound vr	set the sound timer to vr
                         cout << "EMIT: GSOUND V" << x;
#ifdef EMIT_COMMENTS               
                         cout << " ; Equates to ? " << x << " .";
#endif
                         cout << endl;
                         cout << "Not handled yet " << fullInstruction << dec << endl;
                         break;
                    case 0x1E:
                         //I = I + VX
                         cout << "EMIT: ADI V" << x;
#ifdef EMIT_COMMENTS               
                         cout << " ; Equates to ? " << x << " .";
#endif
                         cout << endl;
                         //cout << "Not handled yet " << fullInstruction << dec << endl;
                         break;
                    case 0x29:
                         //FX29
                         //I points to the 4 x 5 font sprite of hex char in VX
                         cout << "EMIT: FONT V" << x;
#ifdef EMIT_COMMENTS               
                         cout << " ; Equates to ? " << x << " .";
#endif
                         cout << endl;
                         //cout << "Not handled yet " << fullInstruction << dec << endl;
                         break;
                    case 0x33:
                         //FX29
                         //I points to the 4 x 5 font sprite of hex char in VX
                         cout << "EMIT: BCD V" << x;
#ifdef EMIT_COMMENTS               
                         cout << " ; Equates to ? " << x << " .";
#endif
                         cout << endl;
                         //cout << "Not handled yet " << fullInstruction << dec << endl;
                         break;
                    case 0x55:
                         //Save V0...VX in memory starting at M(I)
                         //LD[I], V0
                         cout << "EMIT: STR V0-V" << x;
#ifdef EMIT_COMMENTS               
                         cout << " ; Equates to LD[I], V" << x << " in memory starting at M(I).";
#endif
                         cout << endl;

                         //cout << "Not handled yet " << fullInstruction << dec << endl;
                         break;
                    case 0x65:
                         //Load V0...VX from memory starting at M(I)
                         cout << "EMIT: LDR V0-V" << x << ", [I]";
#ifdef EMIT_COMMENTS               
                         cout << " ; Equates to LD V" << x << ", [I] .";
#endif
                         cout << endl;

                         //cout << "Not handled yet " << fullInstruction << dec << endl;
                         break;
                    case 0x75:
                         cout << "Not handled yet " << fullInstruction << dec << endl;
                         break;
                    case 0x85:
                         cout << "Not handled yet " << fullInstruction << dec << endl;
                         break;
                    default:
                         cout << "Not handled yet " << fullInstruction << dec << endl;
                         break;
               }
               break;

     }
     cout << dec;
// */

     return ins;
}



Instruction::Instruction(const Instruction &copyin)   // Copy constructor to handle pass by value.
{
          type = copyin.type;
          memoryOffset = copyin.memoryOffset;
          fullInstruction= copyin.fullInstruction;
          high8 = copyin.high8;
          x = copyin.x;
          y = copyin.y;
          n = copyin.n;
          kk = copyin.kk;
          nnn = copyin.nnn;
          mnemonic = copyin.mnemonic;
          label = copyin.label;
}
Instruction& Instruction::operator=(const Instruction &rhs)
{

     this->type = rhs.type;
     this->memoryOffset = rhs.memoryOffset;
     this->fullInstruction = rhs.fullInstruction;
     this->high8 = rhs.high8;
     this->x = rhs.x;
     this->y = rhs.y;
     this->n = rhs.n;
     this->kk = rhs.kk;
     this->nnn = rhs.nnn;
     this->mnemonic = rhs.mnemonic;
     this->label = rhs.label;

     return *this;
}

int Instruction::operator==(const Instruction &rhs) const
{
     if (this->type != rhs.type) return 0;
     if (this->memoryOffset != rhs.memoryOffset) return 0;
     if (this->fullInstruction != rhs.fullInstruction) return 0;
     if (this->high8 != rhs.high8) return 0;
     if (this->x != rhs.x) return 0;
     if (this->y != rhs.y) return 0;
     if (this->n != rhs.n) return 0;
     if (this->kk != rhs.kk) return 0;
     if (this->nnn != rhs.nnn) return 0;
     if (this->mnemonic != rhs.mnemonic) return 0;
     if (this->label != rhs.label) return 0;
     return 1;
}

// This function is required for built-in STL list functions like sort
int Instruction::operator<(const Instruction &rhs) const
{
     if (this->memoryOffset < rhs.memoryOffset)
     {
          return 1;
     }
     return 0;
}


bool Instruction::isDataOperation()
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
bool Instruction::isLabelOperation()
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

