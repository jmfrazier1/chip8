#include "stdafx.h"
//#include "Memory.h"



Memory::Memory(unsigned int memSize)
{
     _memSize = memSize;
     RAM = (unsigned char *)calloc(_memSize, sizeof(unsigned char));
}

Memory::Memory()
{
     _memSize = 4096;
     RAM = (unsigned char *)calloc(_memSize, sizeof(unsigned char));
}

Memory::~Memory()
{
     if (RAM != NULL)
     {
          free(RAM);
          _memSize = 0;
     }
}

unsigned char Memory::getMem(unsigned int location)
{
     return RAM[location];
}

void Memory::setMem(unsigned int location, unsigned char value)
{
     RAM[location] = value;
}
