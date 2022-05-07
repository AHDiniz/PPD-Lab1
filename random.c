#include <stdlib.h>
#include <time.h>

void InitRandom(void)
{
    srand(time(0));
}

int RandomRange(int low, int high)
{
    return rand() % (high - low + 1) + low;
}
