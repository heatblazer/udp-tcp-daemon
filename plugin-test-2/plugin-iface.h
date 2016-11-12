#ifndef PLUGINIFACE_H
#define PLUGINIFACE_H

struct interface_t
{
    void (*init)();
    int (*put_ndata)(void* data, int len);
    int (*put_data)(void* data);
    void* (*get_data)(void);
    void (*deinit)();

};

extern "C"{
const struct interface_t* get_interface();
}
#endif // PLUGINIFACE_H
