#include "info.h"

void get_cpu_model(char *model) {
    unsigned int eax, ebx, ecx, edx;
    unsigned int *ptr = (unsigned int*)model;

    for (unsigned int i = 0; i < 3; i++) {
        unsigned int leaf = 0x80000002 + i;
        
        __asm__ volatile("cpuid" //не спрашивай, я сам не знаю как это работает
                         : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                         : "a"(leaf));

        *ptr++ = eax;
        *ptr++ = ebx;
        *ptr++ = ecx;
        *ptr++ = edx;
    }
    model[48] = '\0';
}

void get_cpu_vendor(char *vendor) {
    unsigned int eax, ebx, ecx, edx;

    __asm__ volatile("cpuid"  //не спрашивай, я сам не знаю как это работает
                     : "=b"(ebx), "=d"(edx), "=c"(ecx)
                     : "a"(0));

    ((unsigned int*)vendor)[0] = ebx;
    ((unsigned int*)vendor)[1] = edx;
    ((unsigned int*)vendor)[2] = ecx;
    vendor[12] = '\0';
}


