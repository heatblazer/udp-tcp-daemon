#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H
#ifdef __cplusplus
extern "C"{
#endif
// can be declared opque for more flexibility
struct interface_t
{
    void    (*init)();
    void    (*copy)(const void*, void*, int len);
    int     (*put_ndata)(void* data, int len);
    int     (*put_data)(void* data);
    void*   (*get_data)(void);
    void    (*deinit)();
    int     (*main_proxy)(int, char**);
};

const struct interface_t* get_interface();
#ifdef __cplusplus
}
#endif

#endif // PLUGININTERFACE_H
