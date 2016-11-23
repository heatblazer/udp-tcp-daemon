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


static void init()
{
    printf("Init DFT\n");
    printf("Init DFT heap...\n");
    s_list = new queue<int16_t*>();

}

/// perform FFT transform from
/// src to dest by a given len
///
template <typename T> void doFFT(T* dst, const T* src, int len)
{

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
        s_samples[i] = 0;
    }

    doFFT<int16_t>(s_samples, pSmpl, len);

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

