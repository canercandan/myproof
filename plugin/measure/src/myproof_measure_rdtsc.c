#include "myproof_measure.h"

unsigned long myproof_measure_rdtsc()
{
    __asm__("xor %eax, %eax; rdtsc");
}
