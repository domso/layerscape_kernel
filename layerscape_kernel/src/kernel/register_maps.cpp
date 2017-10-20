#include "src/kernel/register_maps.h"

namespace kernel {
    void register_maps::general_registers::operator=(const register_maps::general_registers& old) {
        x30 = old.x30;
        x28 = old.x28;
        x29 = old.x29;
        x26 = old.x26;
        x27 = old.x27;
        x24 = old.x24;
        x25 = old.x25;
        x22 = old.x22;
        x23 = old.x23;
        x20 = old.x20;
        x21 = old.x21;
        x18 = old.x18;
        x19 = old.x19;
        x16 = old.x16;
        x17 = old.x17;
        x14 = old.x14;
        x15 = old.x15;
        x12 = old.x12;
        x13 = old.x13;
        x10 = old.x10;
        x11 = old.x11;
        x8 = old.x8;
        x9 = old.x9;
        x6 = old.x6;
        x7 = old.x7;
        x4 = old.x4;
        x5 = old.x5;
        x2 = old.x2;
        x3 = old.x3;
        x0 = old.x0;
        x1 = old.x1;
    }

    void register_maps::general_registers::clear() {
        x30 = 0;
        x28 = 0;
        x29 = 0;
        x26 = 0;
        x27 = 0;
        x24 = 0;
        x25 = 0;
        x22 = 0;
        x23 = 0;
        x20 = 0;
        x21 = 0;
        x18 = 0;
        x19 = 0;
        x16 = 0;
        x17 = 0;
        x14 = 0;
        x15 = 0;
        x12 = 0;
        x13 = 0;
        x10 = 0;
        x11 = 0;
        x8 = 0;
        x9 = 0;
        x6 = 0;
        x7 = 0;
        x4 = 0;
        x5 = 0;
        x2 = 0;
        x3 = 0;
        x0 = 0;
        x1 = 0;
    }

    void register_maps::fp_registers::operator=(const register_maps::fp_registers& old) {
        d31 = old.d31;
        d30 = old.d30;
        d28 = old.d28;
        d29 = old.d29;
        d26 = old.d26;
        d27 = old.d27;
        d24 = old.d24;
        d25 = old.d25;
        d22 = old.d22;
        d23 = old.d23;
        d20 = old.d20;
        d21 = old.d21;
        d18 = old.d18;
        d19 = old.d19;
        d16 = old.d16;
        d17 = old.d17;
        d14 = old.d14;
        d15 = old.d15;
        d12 = old.d12;
        d13 = old.d13;
        d10 = old.d10;
        d11 = old.d11;
        d8 = old.d8;
        d9 = old.d9;
        d6 = old.d6;
        d7 = old.d7;
        d4 = old.d4;
        d5 = old.d5;
        d2 = old.d2;
        d3 = old.d3;
        d0 = old.d0;
        d1 = old.d1;
    }

    void register_maps::fp_registers::clear() {
        d31 = 0;
        d30 = 0;
        d28 = 0;
        d29 = 0;
        d26 = 0;
        d27 = 0;
        d24 = 0;
        d25 = 0;
        d22 = 0;
        d23 = 0;
        d20 = 0;
        d21 = 0;
        d18 = 0;
        d19 = 0;
        d16 = 0;
        d17 = 0;
        d14 = 0;
        d15 = 0;
        d12 = 0;
        d13 = 0;
        d10 = 0;
        d11 = 0;
        d8 = 0;
        d9 = 0;
        d6 = 0;
        d7 = 0;
        d4 = 0;
        d5 = 0;
        d2 = 0;
        d3 = 0;
        d0 = 0;
        d1 = 0;
    }

    void register_maps::fp_registers::store(register_maps::fp_registers* regs) {
        asm(
            "stp d0, d1, [x0], #16\n"
            "stp d2, d3, [x0], #16\n"
            "stp d4, d5, [x0], #16\n"
            "stp d6, d7, [x0], #16\n"
            "stp d8, d9, [x0], #16\n"
            "stp d10, d11, [x0], #16\n"
            "stp d12, d13, [x0], #16\n"
            "stp d14, d15, [x0], #16\n"
            "stp d16, d17, [x0], #16\n"
            "stp d18, d19, [x0], #16\n"
            "stp d20, d21, [x0], #16\n"
            "stp d22, d23, [x0], #16\n"
            "stp d24, d25, [x0], #16\n"
            "stp d26, d27, [x0], #16\n"
            "stp d28, d29, [x0], #16\n"
            "stp d30, d31, [x0], #16\n"
        );
    }

    void register_maps::fp_registers::load(register_maps::fp_registers* regs) {
        asm(
            "ldp d0, d1, [x0], #16\n"
            "ldp d2, d3, [x0], #16\n"
            "ldp d4, d5, [x0], #16\n"
            "ldp d6, d7, [x0], #16\n"
            "ldp d8, d9, [x0], #16\n"
            "ldp d10, d11, [x0], #16\n"
            "ldp d12, d13, [x0], #16\n"
            "ldp d14, d15, [x0], #16\n"
            "ldp d16, d17, [x0], #16\n"
            "ldp d18, d19, [x0], #16\n"
            "ldp d20, d21, [x0], #16\n"
            "ldp d22, d23, [x0], #16\n"
            "ldp d24, d25, [x0], #16\n"
            "ldp d26, d27, [x0], #16\n"
            "ldp d28, d29, [x0], #16\n"
            "ldp d30, d31, [x0], #16\n"
        );
    }

    void register_maps::system_registers::operator=(const register_maps::system_registers& old) {
        SP_EL0 = old.SP_EL0;
        ELR_EL1 = old.ELR_EL1;
        TPIDR_EL0 = old.TPIDR_EL0;
        TPIDR_EL1 = old.TPIDR_EL1;
        TPIDRRO_EL0 = old.TPIDRRO_EL0;
        SPSR_EL1 = old.SPSR_EL1;
    }

    void register_maps::system_registers::clear() {
        SP_EL0 = 0;
        ELR_EL1 = 0;
        TPIDR_EL0 = 0;
        TPIDR_EL1 = 0;
        TPIDRRO_EL0 = 0;
        SPSR_EL1 = 0;
    }

    void register_maps::system_registers::store(register_maps::system_registers* regs) {
        asm(
            "mrs     x1, SP_EL0\n"
            "str     x1, [x0], #8\n"

            "mrs     x1, ELR_EL1\n"
            "str     x1, [x0], #8\n"

            "mrs     x1, TPIDR_EL0\n"
            "str     x1, [x0], #8\n"

            "mrs     x1, TPIDR_EL1\n"
            "str     x1, [x0], #8\n"

            "mrs     x1, TPIDRRO_EL0\n"
            "str     x1, [x0], #8\n"

            "mrs     x1, SPSR_EL1\n"
            "str     x1, [x0], #8\n"
        );
    }

    void register_maps::system_registers::load(register_maps::system_registers* regs) {
        asm(
            "ldr     x1, [x0], #8\n"
            "msr     SP_EL0, x1\n"

            "ldr     x1, [x0], #8\n"
            "msr     ELR_EL1, x1\n"

            "ldr     x1, [x0], #8\n"
            "msr     TPIDR_EL0, x1\n"

            "ldr     x1, [x0], #8\n"
            "msr     TPIDR_EL1, x1\n"

            "ldr     x1, [x0], #8\n"
            "msr     TPIDRRO_EL0, x1\n"

            "ldr     x1, [x0], #8\n"
            "msr     SPSR_EL1, x1\n"
        );
    }
}
