#include <iostream>
#include "plugin-interface.h"

using namespace std;

int main(int argc, char *argv[])
{
    const interface_t* dft = get_interface();
    dft->main_proxy(argc, argv);
    dft->init();
    dft->deinit();
    return 0;
}

