#include "plugin-interface.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

// needed for kissfft c++ //
#include <iostream>
#include <cstdlib>
#include <typeinfo>

#include "External/kiss_fft.h" // c++ api

template <typename T> struct node_t
{
    // copy ctor to be implemented
    T data;
    struct node_t* next;
};


template <typename T> class queue
{
    size_t m_size;
public:
    queue() : m_size(0) , first(nullptr), last(nullptr){}
    ~queue() {}

    void enqueue(T data)
    {
        m_size++;

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
        m_size--;
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

    size_t count() { return m_size; }
    bool empty() { return (first == nullptr ); }

private:
    node_t<T> *first, *last;
};

///////////////////////////////////////////////////////////////////////////////////
static int16_t* s_samples = nullptr;
static queue<int16_t*>* s_list = nullptr;
///////////////////////////////////////////////////////////////////////////////////


static void init()
{
    printf("Init DFT\n");
    printf("Init DFT heap...\n");
    s_list = new queue<int16_t*>();
}


static void check(kiss_fft_cpx* in, kiss_fft_cpx* out, int len)
{
    double errpow = 0, sigpow = 0;

    for(int i=0; i < len; ++i) {
        double ansreal = 0;
        double ansimag = 0;
        double difreal;
        double difimag;

        for(int k=0; k < len; ++k) {
            double phase = -2.0 * M_PI * i * k / len;
            double real =       cos(phase);
            double imaginary =  sin(phase);
#ifdef FIXED_POINT
            real /= len;
            imaginary /= len;
#endif
            ansreal += in[k].r * real - in[k].i * imaginary;
            ansimag += in[k].r * imaginary + in[k].i * real;
        }

        difreal = ansreal - out[i].r;
        difimag = ansimag - out[i].i;

        errpow += (difreal * difreal) + (difimag * difimag);
        sigpow += (ansreal * ansreal) + (ansimag * ansimag);
    }
}


static void doDFT(int16_t* in, int len)
{
    int16_t *in2 = nullptr;
    in2 = new int16_t[len];

    double arg = 0, cosarg = 0, sinarg = 0;

    for(int i=0; i < len; ++i) {
        in2[i] = 0;
        arg =  -2.0 * 3.141592654 * ((double) i) / ((double) len);

        for(int k=0; k < len; ++k) {
            sinarg = sin(k * arg); // imaginare part
            cosarg = cos(k * arg); // real part
            printf("[%f]\n", (in[k] * cosarg) - (in[k] * sinarg));
            in2[k] += (in[k] * cosarg) - (in[k] * sinarg);
        }
        puts("-------\n");
    }

    for (int i=0; i < len; ++i) {
        in[i] = in2[i] / (double) len;
    }

    delete [] in2;
    in2 = nullptr;
}

/// just copy it
/// \brief put_ndata
/// \param data
/// \param len
/// \return
///
static int put_ndata(void *data, int len)
{
    if (s_samples != nullptr) {
        delete s_samples;
        s_samples = nullptr;
    } else {
        s_samples = new int16_t[len];
        memset(s_samples, 0, len);
    }

    int16_t* pData = (int16_t*) data;

    for(int i=0; i < len; ++i) {
        s_samples[i] = pData[i];
    }

    doDFT(s_samples, len);

    return 0;
}




static int put_data(void *data)
{
    (void) data;
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
    if (s_list != nullptr) {
        printf("Deinit DFT heap...\n");
        while (!s_list->empty()) {
            (void) s_list->dequeue();
        }
        delete s_list;
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

