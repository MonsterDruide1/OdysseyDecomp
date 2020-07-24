#include <nn/os.h>

int doStuff()
{
    nn::os::AllocateMemoryBlock(nullptr, 0x100);
    int i = 0;
    return i;
}