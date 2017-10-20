#include "src/modules/scheduler/el0_start.h"

namespace module {
    namespace scheduler {
        void el0_start(byte* code, byte* stack, uint64_t SPSR, uint64_t stackOffset, kernel::register_maps::general_registers* regs) {
            asm(
                "isb    sy\n"
                "dsb    st\n"
                "cmp    x3, xzr\n"
                "b.eq   stack_restore_end\n"
                "mov    sp, x3\n"
                "stack_restore_end:\n"
                "msr    ELR_EL1, x0\n"
                "msr    SP_EL0, x1\n"
                "msr    SPSR_EL1, x2 \n"
                "mov    x1, x4\n"

                "ldr x30, [x1], #16\n"
                "ldr x28, [x1], #8\n"
                "ldp x29, x26, [x1], #16\n"
                "ldp x27, x24, [x1], #16\n"
                "ldp x25, x22, [x1], #16\n"
                "ldp x23, x20, [x1], #16\n"
                "ldp x21, x18, [x1], #16\n"
                "ldp x19, x16, [x1], #16\n"
                "ldp x17, x14, [x1], #16\n"
                "ldp x15, x12, [x1], #16\n"
                "ldp x13, x10, [x1], #16\n"
                "ldp x11, x8, [x1], #16\n"
                "ldp x9, x6, [x1], #16\n"
                "ldp x7, x4, [x1], #16\n"
                "ldp x5, x2, [x1], #16\n"
                "ldp x3, x0, [x1], #16\n"
                "ldr x1, [x1]\n"

                "eret\n"
            );
        }
    }
}
