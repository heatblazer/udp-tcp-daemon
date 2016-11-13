#include "plugin-interface.h"

int main(int argc, char *argv[])
{

    const interface_t* sniff = get_interface();

    sniff->init();
    sniff->deinit();
    return 0;
}
