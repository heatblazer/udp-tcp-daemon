#include "plugin-interface.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


static int16_t* s_samples = nullptr;

void static DFT(int16_t* src, int N)
{
    int n, k; // indexes for time and freq domain
    int16_t* x_real = new int16_t[N];
    int16_t* x_imaginery = new int16_t[N];
    int16_t* p_spectrum = new int16_t[N];

    for(k=0; k < N; k++) {
        // calculate real and imaginery parts
        x_real[k] = 0;
        for(int i=0; i < N; ++i) {
            x_real[i] += (src[i] * cos(i * k * M_PI_2 / N));
        }

        x_imaginery[k] = 0;
        for(int i=0; i < N; ++i) {
            x_imaginery[i] -= (float) (src[i] * sin(i * k * M_PI_2/ N));
        }

        p_spectrum[k] = (x_imaginery[k]*x_imaginery[k]) + (x_real[k]*x_real[k]);
    }

    for(int i=0; i < N; ++i) {
        src[i] = p_spectrum[i];
    }
}


static void init()
{
    printf("Init DFT\n");
}

static int put_ndata(void *data, int len)
{
    if (s_samples) {
        delete [] s_samples;
        s_samples = nullptr;
    }
    s_samples = new int16_t[len];
    int16_t* pSmpl = (int16_t*) data;
    for(int i=0; i < len; ++i) {
        s_samples[i] = pSmpl[i];
    }

    DFT(s_samples, len);
}

static int put_data(void *data)
{
    return 0;
}

static void *get_data()
{
    return (int16_t*) s_samples;
}

static void deinit()
{
    printf("Deinit DFT\n");
}

static int main_proxy(int argc, char** argv)
{
    (void) argc;
    (void) argv;
    printf("DFT: No args main...\n");
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

