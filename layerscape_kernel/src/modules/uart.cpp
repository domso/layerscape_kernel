#include "src/modules/uart.h"
#include "src/modules/general_interrupt_controller.h"
#include "src/modules/interrupt_handler.h"

namespace module {
    bool uart::load() {
        enable_interrupts();
        kernel::system::register_stdout(&print);

        module::interrupt_handler::register_interrupt_handler<input_interrupt>();

        m_mutex.init();

        return true;
    }

    void uart::print(const char* string) {
        int64_t i = 0;
        volatile registers* reg = PORT1;

        while (string[i] != '\0') {
            while ((reg->LSR & 0x20) == 0) {}

            reg->THR_RBR_DLL = string[i];
            i++;
        }

//             while ((reg->LSR & 0x40) == 0) {}
    }

    void uart::println(const char* string) {
        m_mutex.lock();
        print(string);
        print("\r\n");
        m_mutex.unlock();
    }

    char uart::getc() {
        if ((PORT1->LSR & 0x1) == 0) {
            return 0;
        }

        return PORT1->THR_RBR_DLL;
    }

    void uart::putc(const char c) {
        while ((PORT1->LSR & 0x20) == 0) {}

        PORT1->THR_RBR_DLL = c;
    }

    void uart::input_interrupt::handle(kernel::register_maps::general_registers& saved_registers) {
        char c = module::uart::getc();
        module::uart::putc(c);
    }

    void uart::enable_interrupts() {
        PORT1->IER_DLH = 1;
        module::general_interrupt_controller::interface::distributor->GICD_ISENABLER[2]   = 0x00000001;
    }

    util::mutex uart::m_mutex;
}
