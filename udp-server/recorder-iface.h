#ifndef RECORDERIFACE_H
#define RECORDERIFACE_H

namespace iz {

struct RecIface
{
    void (*init)(void);
    int (*put_ndata)(void*, int);
    int (*put_data)(void*);
    void* (*get_data)(void);
    void (*deinit)(void);
    int (*main_proxy)(int, char**); // if we need to call lib in main
};

} // iz

#endif // RECORDERIFACE_H
