#ifndef kernel_register_maps_h
#define kernel_register_maps_h

#include <src/stddef.h>

namespace kernel {
    namespace register_maps {
        // is saved by the asm-exception-handler in startup.S
        struct general_registers {
            uint64_t x30;
            uint64_t dummy;
            uint64_t x28;
            uint64_t x29;
            uint64_t x26;
            uint64_t x27;
            uint64_t x24;
            uint64_t x25;
            uint64_t x22;
            uint64_t x23;
            uint64_t x20;
            uint64_t x21;
            uint64_t x18;
            uint64_t x19;
            uint64_t x16;
            uint64_t x17;
            uint64_t x14;
            uint64_t x15;
            uint64_t x12;
            uint64_t x13;
            uint64_t x10;
            uint64_t x11;
            uint64_t x8;
            uint64_t x9;
            uint64_t x6;
            uint64_t x7;
            uint64_t x4;
            uint64_t x5;
            uint64_t x2;
            uint64_t x3;
            uint64_t x0;
            uint64_t x1;

            void operator=(const general_registers& old);
            void clear();
        };

        // needs to be saved manually
        struct fp_registers {
            uint64_t d0;
            uint64_t d1;
            uint64_t d2;
            uint64_t d3;
            uint64_t d4;
            uint64_t d5;
            uint64_t d6;
            uint64_t d7;
            uint64_t d8;
            uint64_t d9;
            uint64_t d10;
            uint64_t d11;
            uint64_t d12;
            uint64_t d13;
            uint64_t d14;
            uint64_t d15;
            uint64_t d16;
            uint64_t d17;
            uint64_t d18;
            uint64_t d19;
            uint64_t d20;
            uint64_t d21;
            uint64_t d22;
            uint64_t d23;
            uint64_t d24;
            uint64_t d25;
            uint64_t d26;
            uint64_t d27;
            uint64_t d28;
            uint64_t d29;
            uint64_t d30;
            uint64_t d31;

            void operator=(const fp_registers& old);
            static void load(fp_registers* regs);
            static void store(fp_registers* regs);
            void clear();
        };

        struct system_registers {
            byte* SP_EL0;
            byte* ELR_EL1;
            uint64_t TPIDR_EL0;
            uint64_t TPIDR_EL1;
            uint64_t TPIDRRO_EL0;
            uint64_t SPSR_EL1;

            void operator=(const system_registers& old);
            static void load(system_registers* regs);
            static void store(system_registers* regs);
            void clear();
        };

    }
}

#endif
