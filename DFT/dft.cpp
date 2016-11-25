#include "plugin-interface.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

// needed for kissfft c++ //
#include <iostream>
#include <cstdlib>
#include <typeinfo>

#include "External/kissfft.hh" // c++ api

template <typename T> struct node_t
{
    // copy ctor to be implemented
    T data;
    struct node_t* next;
};


template <typename T> class queue
{
public:
    queue() : first(nullptr), last(nullptr){}
    ~queue() {}

    void enqueue(T data)
    {
        node_t<T> *n = new node_t<T>();
        n->data = data;
        n->next = nullptr;

        if (first == nullptr) {
            first = n;
            last = first;
        } else {
            last->next = n;
            last = n;
        }
    }

    T dequeue()
    {
        if (first == nullptr) {
            // empty
            T none;
            return none;
        } else {
            node_t<T>* deleteme = first;
            first = first->next;
            T ret = deleteme->data;
            delete deleteme;
            return ret;
        }
    }

    bool empty() { return (first == nullptr ); }
private:
    node_t<T>* first, *last;
};

static int16_t* s_samples = nullptr;

static queue<int16_t*>* s_list = nullptr;

#define SIN_2PI_16 0.38268343236508978
#define SIN_4PI_16 0.707106781186547460
#define SIN_6PI_16 0.923879532511286740
#define C_P_S_2PI_16 1.30656296487637660
#define C_M_S_2PI_16 0.54119610014619690
#define C_P_S_6PI_16 1.3065629648763766
#define C_M_S_6PI_16 -0.54119610014619690


static void init()
{
    printf("Init DFT\n");
    printf("Init DFT heap...\n");
    s_list = new queue<int16_t*>();

}

/// perform FFT transform from
/// src to dest by a given len
///
template <typename T> void doFFT(T* dst, const T* src, int len=16)
{
    T temp, out0, out1, out2, out3, out4, out5, out6, out7,
            out8, out9, out10, out11, out12, out13, out14, out15;

    out0 = src[0] + src[8];
    out1 = src[1] + src[9];
    out2 = src[2] + src[10];
    out3 = src[3] + src[11];
    out4 = src[4] + src[12];
    out5 = src[5] + src[13];
    out6 = src[6] + src[14];
    out7 = src[7] + src[15];

    // butterfly here
    // [1]  [8]  []
    //    \/   \/
    //    /\   /\
    // [2]  [9]  []
    //
    out8 = src[0] - src[8];
    out9 = src[1] - src[9];
    out10 = src[2] - src[10];
    out11 = src[3] - src[11];
    out12 = src[12] - src[4];
    out13 = src[13] - src[5];
    out14 = src[14] - src[6];
    out15 = src[15] - src[7];

    temp = (out13 - out9) * (SIN_2PI_16);
    out9 = out9 * (C_P_S_2PI_16) + temp;
    out13 = out13 * (C_M_S_2PI_16) + temp;

    out14 *= (SIN_4PI_16);
    out10 *= (SIN_4PI_16);
    out14 = out14 - out10;
    out10 = out14 + out10 + out10;

    temp = (out15 - out11) * (SIN_6PI_16);
    out11 = out11 * (C_P_S_6PI_16) + temp;
    out15 = out15 * (C_M_S_6PI_16) + temp;

    out8 += out10;
    out10 = out8 - out10 - out10;

    out12 += out14;
    out14 = out12 - out14 - out14;

    out9 += out11;
    out11 = out9 - out11 - out11;

    out13 += out15;
    out15 = out13 - out15 - out15;

    dst[1] = out8 + out9;
    dst[7] = out8 - out9;

    dst[9] = out12 + out13;
    dst[15] = out12 - out13;

    dst[5] = out10 + out15;
    dst[13] = out14 - out11;
    dst[3] = out10 - out15;
    dst[11] = -out14 - out11;

    out0 = out0 + out4;
    out4 = out0 - out4 - out4;
    out1 = out1 + out5;
    out5 = out1 - out5 - out5;
    out2 += out6;
    out6 = out2 - out6 - out6;
    out3 += out7;
    out7 = out3 - out7 - out7;

    dst[0] = out0 + out2;
    dst[4] = out0 - out2;
    out1 += out3;
    dst[12] = out3 + out3 - out1;
    dst[0] += out1;
    dst[8] = dst[0] - out1 - out1;

    out5 *= SIN_4PI_16;
    out7 *= SIN_4PI_16;
    out5 = out5 - out7;
    out7 = out5 + out7 + out7;

    dst[14] = out6 - out7;
    dst[2] = out5 + out4;
    dst[6] = out4 - out5;
    dst[10] = -out7 - out6;

}

/// just copy it
/// \brief put_ndata
/// \param data
/// \param len
/// \return
///
static int put_ndata(void *data, int len)
{

    s_samples = (int16_t*) data;
    //doFFT<int16_t>(s_samples, s_samples, len);

}




static int put_data(void *data)
{
    s_list->enqueue((int16_t*) data);
}

static void *get_data()
{
    if (s_samples) {
        return (int16_t*) s_samples;
    } else {
        return nullptr;
    }
}

static void deinit()
{
    printf("Deinit DFT\n");
    printf("Deinit DFT heap...\n");
    while (!s_list->empty()) {
        (void) s_list->dequeue();
    }
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

