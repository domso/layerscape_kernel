#include "src/modules/hang.h"
#include "src/modules/uart.h"

#include "src/modules/general_interrupt_controller.h"

namespace module {
    bool hang::load() {
        kernel::system::register_start_application(loop);

        return true;
    }

    void hang::loop() {
        while (true) {
            for (int i = 0; i < 10000; i++) {
                for (int j = 0; j < 1000; j++) {

                }
            }

            char output;

            if (kernel::system::is_core0()) {
                output = '.';
            } else {
                output = '-';
            }

//             module::uart::printHex(module::general_interrupt_controller::interface::redistributor->sgi_base.GICR_ICENABLER[0]);

            module::uart::putc(output);
        }
    }

}
