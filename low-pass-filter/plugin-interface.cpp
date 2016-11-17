#include "plugin-interface.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// local vars
uint16_t* s_Samples = NULL;

float* s_Smooth = NULL;
float LPF_Beta = 0.025f;

static void init()
{
    printf("Init Low Pass Filter\n");
}

static int put_ndata(void *data, int len)
{
    uint16_t* samples = (uint16_t*) data;

    if (s_Smooth) {
        free(s_Smooth);
        s_Smooth = NULL;
    }

    s_Smooth = (float*) malloc(sizeof(float) * len);
    memset(s_Smooth, 0, len);

    if (!s_Smooth) {
        return -1;
    }

    for(int i=0; i < len; ++i) {
        s_Smooth[i] = s_Smooth[i] -
                (LPF_Beta * (s_Smooth[i] - samples[i]));
    }

    return len;
}

static int put_data(void *data)
{
    return 0;
}

static void *get_data()
{
    return s_Smooth;
}

static void deinit()
{
    printf("Deinit Low Pass Filter\n");
}

static int main_proxy(int argc, char** argv)
{
    (void) argc;
    (void) argv;
    printf("LowPassFilter: No args main...\n");
    return 0;
}

static struct interface_t s_iface;

const struct interface_t *get_interface()
{
    s_iface.init = &init;
    s_iface.deinit = &deinit;
    s_iface.get_data = &get_data;
    s_iface.put_data = &put_data;
    s_iface.put_ndata = &put_ndata;
    s_iface.main_proxy = &main_proxy;
    return &s_iface;
}

