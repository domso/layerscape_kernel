#include "src/modules/exceptions.h"
#include "src/modules/syscall/controller.h"
#include "src/modules/interrupt_handler.h"
#include "src/modules/uart.h"
#include "src/modules/general_interrupt_controller.h"
#include "src/modules/scheduler/controller.h"

namespace module {
    bool exceptions::load() {
        kernel::system::register_exception_sync(&error_msg);
        kernel::system::register_exception_FIQ(&error_msg);
        kernel::system::register_exception_SError(&error_msg);

        kernel::system::register_exception_IRQ(&interrupt);

        return true;
    }

    void exceptions::error_msg(byte* sp) {
        module::uart::println("[KERNEL PANIC]");

        while (true) {}
    }

    void exceptions::interrupt(byte* sp) {
        uint32_t esr;
        uint32_t iar;
        kernel::register_maps::general_registers* registers = (kernel::register_maps::general_registers*)sp;

        asm(
            "mrs    %[esr], ESR_EL1\n"
            "msr    ESR_EL1, xzr\n"
            : [esr] "=r"(esr)
        );

        if ((esr & 0x56000000) == 0x56000000) {
            // syscall
            if (!module::syscall::controller::execute(*registers)) {
                module::uart::println("[ERROR] Error while executing syscall #");
                module::uart::printHex(registers->x8);
                module::uart::println("");
            }
        } else if (esr == 0) {
            // GIC
            iar = module::general_interrupt_controller::interface::cpu::ICC_IAR1_EL1::load();

            if (!module::interrupt_handler::execute(iar, *registers)) {
                module::uart::print("[ERROR] Error while executing handler for interrupt #");
                module::uart::printHex(iar);
                module::uart::println("");
            }

            module::general_interrupt_controller::interface::cpu::ICC_EOIR1_EL1::store(iar);
        } else {
            uint64_t elr;

            asm(
                "mrs    %[elr], ELR_EL1\n"
                : [elr] "=r"(elr)
            );

            module::uart::print("[FATAL ERROR] ESR_EL1: ");
            module::uart::printHex(esr);
            module::uart::print(" ELR_EL1: ");
            module::uart::printHex(elr);
            module::uart::print("\r\n");

            auto process = module::scheduler::controller::get_current_process();

            if (process != nullptr) {
                process->signal(module::scheduler::process_status::signals::SIGTERM);
                module::scheduler::controller::context_switch(*registers);
            }
        }

        // NOTE:
        // A syscall/interrupt may branch to a different address
        // --> the stackpointer will be restored using sp
        // --> no other clean-up-task may be performed!
        // --> this point may be unreachable!

    }
}
