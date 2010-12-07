#include "myproof_measure.h"

inline unsigned long myproof_measure_rdtsc()
{
    __asm__ ("xor %eax, %eax; cpuid; rdtsc");
}
