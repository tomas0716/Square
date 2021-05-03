#pragma once

#ifdef _DEBUG
#pragma comment(lib,"../lib/GwLib_D.lib")
#else
#pragma comment(lib,"../lib/GwLib.lib")
#endif

//#pragma comment(lib,"ws2_32.lib")

#include "../Include/GwLib.h"/*
#include "../Include/Util/GwWStringShifter.h"
#include "../Include/Util/GwStringUtil.h"*/


#ifdef _DEBUG
#pragma comment(lib, "../Lib/EsLib_D.lib")
#else
#pragma comment(lib, "../Lib/EsLib.lib")
#endif

#include "../EsLib/Include.h"

//#include <ctime>
//#include <limits>