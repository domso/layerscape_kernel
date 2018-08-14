#include "src/kernel/system.h"
#include <stdlib.h>

void module_list();


// const bool secondaryCore
extern "C"
void c_entry() {
//     if (!secondaryCore) {
        kernel::system::init();
//     }
    
    module_list();
    kernel::system::start_registered_app();

    // should never happen!
    while (true) {}
}
