#pragma once
#include "stdafx.h"
//#include <string>
using namespace std;

class Register
{
private:
     string _name;
     unsigned char _value;
public:

     Register();

     Register(string name);
     ~Register();

     void setValue(unsigned char value);
     unsigned char getValue();


};

