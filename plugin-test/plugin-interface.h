#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

// can be declared opque for more flexibility
struct interface_t
{
    void (*init)();
    int (*put_ndata)(void* data, int len);
    int (*put_data)(void* data);
    void* (*get_data)(void);
    void (*deinit)();

};
extern "C" {
const struct interface_t* get_interface();
}

#endif // PLUGININTERFACE_H
