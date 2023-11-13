#pragma once
/*
Add this line to cpp files BEFORE THE INCLUDE FILES to test for location of memory leaks:

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

and add this header file to the cpp files too

*/
#include "crtdbg.h"

#ifdef _DEBUG
#define DEBUG_NEW   new( _NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_NEW
#endif