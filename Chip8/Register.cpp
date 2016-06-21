#include "stdafx.h"
//#include "Register.h"


Register::Register()
{
     _name = "";
}

Register::Register(string name)
{
     _name = name;
}

void Register::setValue(unsigned char value)
{
     _value = value;
}
unsigned char Register::getValue()
{
     return _value;
}

Register::~Register()
{
}
