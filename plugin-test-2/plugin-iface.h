#ifndef PLUGINIFACE_H
#define PLUGINIFACE_H

#ifdef __cplusplus
extern "C"{
#endif
struct interface_t
{
    void (*init)();
    int (*put_ndata)(void* data, int len);
    int (*put_data)(void* data);
    void* (*get_data)(void);
    void (*deinit)();

};

const struct interface_t* get_interface();
#ifdef __cplusplus
}
#endif

#endif // PLUGINIFACE_H
