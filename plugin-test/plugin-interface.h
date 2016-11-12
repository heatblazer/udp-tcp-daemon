#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

extern void init();
extern int put_ndata(void* data, int len);
extern int put_data(void* data);
extern void* get_data(void);
extern void deinit();

#endif // PLUGININTERFACE_H
