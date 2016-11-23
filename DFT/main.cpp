//////////// THIS IS A UNIT LIB TEST //////////////

#include <iostream>
#include "plugin-interface.h"

using namespace std;

int main(int argc, char *argv[])
{
    const interface_t* dft = get_interface();
    dft->main_proxy(argc, argv);
    dft->init();

    int16_t smpl[4] = {1,2,3,4};

    dft->put_ndata((int16_t*)smpl, 4);
    int16_t* next = (int16_t*)dft->get_data();

    for(int i=0; i < 4; ++i) {
        cout << next[i] << endl;
    }

    dft->deinit();
    return 0;
}

