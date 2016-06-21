// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"


#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <list>

#include "Instruction.h"
#include "VirtualMachine.h"
#include "Register.h"
#include "Memory.h"
#include "ViewPort.h"


// TODO: reference additional headers your program requires here

//This defines statement is used to suppress logging messages and run the 
//display that would be seen on a chip 8 graphics machine.
#define SUPPRESS_ALL_OUTPUT
