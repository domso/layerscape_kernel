#ifndef modules_scheduler_el0_start_h
#define modules_scheduler_el0_start_h

#include <src/stddef.h>
#include "src/kernel/register_maps.h"

namespace module {
    namespace scheduler {
        // restores the stackpointer to 'stackOffset' and starts code from el0 with the 
        // given context
        void el0_start(byte* code, byte* stack, uint64_t SPSR, uint64_t stackOffset, kernel::register_maps::general_registers* regs);
    }
}

#endif
