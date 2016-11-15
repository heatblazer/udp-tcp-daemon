#include "plugin-interface.h"
#include <stdio.h>


static void init()
{
    printf("Init Plugin1!\n");
    printf("init()\n");
}

static int put_ndata(void *data, int len)
{
    printf("put_ndata(%p, %d)\n", data, len);
    return 0;
}

static int put_data(void *data)
{
    printf("put_data(%p)\n", data);
    return 0;
}

static void *get_data()
{
    printf("get_data()\n");
    return 0x00;
}

static void deinit()
{
    printf("deinit()\n");
}

static int main_proxy(int argc, char** argv)
{
    (void) argc;
    (void) argv;
    printf("No args main...\n");
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

