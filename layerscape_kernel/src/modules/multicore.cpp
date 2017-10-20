#include "src/modules/multicore.h"
#include "src/modules/mmu/controller.h"
#include "src/modules/general_interrupt_controller.h"

namespace module {
    bool multicore::load() {
        kernel::system::set_core_count(8);
        // U-Boot let all secondary cores spin at the given address!
        // The addresses could be obtained by the 'cpu status'-command in U-Boot,
        // but the view is buggy and does not show the correct address of all cores!
        start_cpu(kernel::system::page_alloc() + kernel::system::page_size(), (byte*) 0x00000000fff0c5f0);
        start_cpu(kernel::system::page_alloc() + kernel::system::page_size(), (byte*) 0x00000000fff0c6b0);
        start_cpu(kernel::system::page_alloc() + kernel::system::page_size(), (byte*) 0x00000000fff0c6f0);
        start_cpu(kernel::system::page_alloc() + kernel::system::page_size(), (byte*) 0x00000000fff0c7b0);
        start_cpu(kernel::system::page_alloc() + kernel::system::page_size(), (byte*) 0x00000000fff0c7f0);
        start_cpu(kernel::system::page_alloc() + kernel::system::page_size(), (byte*) 0x00000000fff0c8b0);
        start_cpu(kernel::system::page_alloc() + kernel::system::page_size(), (byte*) 0x00000000fff0c8f0);

        wake_up();

        return true;
    }

    void multicore::wake_up() {
        // U-Boot uses 'wfi' instructions for sleeping
        asm volatile("dsb st");
        asm volatile("isb");
        asm volatile("sev");

        // lets try all Software-Interrupts
        // could be reduced to only 1 interrupt
        module::general_interrupt_controller::interface::cpu::ICC_ASGI1R_EL1::store((uint64_t)1 << 40);
        module::general_interrupt_controller::interface::cpu::ICC_SGI0R_EL1::store((uint64_t)1 << 40);
        module::general_interrupt_controller::interface::cpu::ICC_SGI1R_EL1::store((uint64_t)1 << 40);
    }

    void multicore::start_cpu(byte* stack, byte* spinAddress) {
        module::mmu::controller::disable_mmu();
        asm(
            "b start_secondary_cpu_end\n"

            "start_secondary_cpu:\n"
            "ldr    x19, [x11, #8]\n"
            "mov    x20, #1\n"
            "b  init_el2\n"

            "start_secondary_cpu_end:\n"
            "ldr    x3, =start_secondary_cpu\n"

            "str    x0, [x1, #8]\n"
            "isb\n"
            "dsb st\n"
            "str    x3, [x1]\n"
        );
        module::mmu::controller::enable_mmu();
    }
}
