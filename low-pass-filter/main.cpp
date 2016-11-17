#if 0
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "plugin-interface.h"

#define SIZE (32 * 16)

int main(int argc, char *argv[])
{

    const interface_t* iface = get_interface();
    uint16_t samples[SIZE] = {0};
    for(int i=0; i < SIZE; ++i) {
        samples[i] = i;
    }

    for(int i=0; i < 16; ++i) {
        iface->put_ndata(samples, SIZE);
    }

    puts("\n");
    uint16_t* smp = (uint16_t*) iface->get_data();
    for(int i=0; i < SIZE; ++i) {
        printf("[%d]", smp[i]);
    }
    puts("\n");
    return 0;
}
#endif
