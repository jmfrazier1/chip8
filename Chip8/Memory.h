#pragma once
#include "stdafx.h"
class Memory
{

private:
     unsigned char *RAM;
     unsigned int _memSize;
public:
     Memory(unsigned int memSize);
#ifdef WONT_BE
     {
          _memSize = memSize;
          RAM = (unsigned char *)calloc(_memSize, sizeof(unsigned char));
     }
#endif
     Memory();
#ifdef WONT_BE
     {
          _memSize = 4096;
          RAM = (unsigned char *)calloc(_memSize, sizeof(unsigned char));
     }
#endif
     ~Memory();
#ifdef WONT_BE
     {
          if (RAM != NULL)
          {
               free(RAM);
               _memSize = 0;
          }
     }
#endif

     unsigned char getMem(unsigned int location);
#ifdef WONT_BE
     {
          return RAM[location];
     }
#endif
     void setMem(unsigned int location, unsigned char value);
#ifdef WONT_BE
     {
          RAM[location] = value;
     }
#endif
};

