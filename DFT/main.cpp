//////////// THIS IS A UNIT LIB TEST //////////////

#include <iostream>
#include "plugin-interface.h"

using namespace std;

int main(int argc, char *argv[])
{
    const interface_t* dft = get_interface();
    dft->main_proxy(argc, argv);
    dft->init();
    int16_t samples[16] ;
    for(int i=0; i < 16; ++i) {
        samples[i] = 1;
    }

    dft->put_ndata((int16_t*)samples, 16);

    int16_t* pSmpl = (int16_t*)dft->get_data();

    for(int i=0; i < 16; ++i) {
        std::cout << "[" << pSmpl[i] << "]";
    }
    dft->deinit();
    return 0;
}

