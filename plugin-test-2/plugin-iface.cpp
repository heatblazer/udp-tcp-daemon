#include "plugin-iface.h"
#include <stdio.h>


static void init()
{
    printf("Init Plugin 2!\n");
    printf("init()\n");
}

int put_ndata(void *data, int len)
{
    (void) data;
    (void) len;
    printf("put_ndata()\n");
    return 0;
}

static int put_data(void *data)
{
    printf("put_data(%p)\n", data);
    return 0;
}

static void *get_data()
{
    printf("get a nullptr test data!\n");
    printf("get_data()\n");
    return 0x00;
}

static void deinit()
{
    printf("Deinit plugin 2!\n");
    printf("deinit()\n");
}

static int main_proxy(int argc, char** argv)
{
    (void) argc;
    (void) argv;
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
