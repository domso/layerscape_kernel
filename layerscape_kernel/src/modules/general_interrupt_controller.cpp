#include "src/modules/general_interrupt_controller.h"

namespace module {
    bool general_interrupt_controller::load() {
        module::general_interrupt_controller::interface::cpu::ICC_SRE_EL1::store(0x7);
    }

    volatile module::general_interrupt_controller::interface::redistributor_map* general_interrupt_controller::interface::redistributor() {
        uint32_t id = kernel::system::MPIDR();

        for (volatile redistributor_map* current = redistributor_base; current < redistributor_base_end; current++) {
            if (id == (current->rd_base.GICR_TYPER >> 32)) {
                return current;
            }
        }

        //should never reach this point
        return nullptr;
    }

}
