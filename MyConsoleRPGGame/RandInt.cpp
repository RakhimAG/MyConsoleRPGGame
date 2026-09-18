#include "RandInt.h"
#include "cstdlib"


int RandInt(int start, int end)
{
    return rand() % (end - start + 1) + start;
}
