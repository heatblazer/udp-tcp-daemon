#include "plugin-interface.h"
#include <stdio.h>


void init()
{
    printf("init()\n");
}

int put_ndata(void *data, int len)
{
    printf("put_ndata(%p, %d)\n", data, len);
    return 0;
}

int put_data(void *data)
{
    printf("put_data(%p)\n", data);
    return 0;
}

void *get_data()
{
    printf("get_data()\n");
    return 0x00;
}
