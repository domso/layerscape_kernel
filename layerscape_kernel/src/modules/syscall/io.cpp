#include "src/modules/syscall/io.h"
#include "src/modules/syscall/controller.h"
#include "src/modules/uart.h"

#include "src/modules/general_interrupt_controller.h"
#include "src/modules/interrupt_handler.h"

namespace module {
    namespace syscall {
        bool io::load() {
            controller::register_syscall<read>();
            controller::register_syscall<write>();

            module::interrupt_handler::register_interrupt_handler<input_interrupt>();

            m_readBufferPos = 0;
            m_readBuffer[m_readBufferSize] = '\0';
            m_mutex.init();

            return true;
        }

        void io::read::handle(kernel::register_maps::general_registers& saved_register) {
            m_mutex.lock();
            char* output = (char*) saved_register.x0;
            saved_register.x0 = 0;

            for (int i = 0; i < m_readBufferPos; i++) {
                if (saved_register.x1 > 0) {
                    output[i] = m_readBuffer[i];
                    m_readBuffer[i] = '\0';
                    saved_register.x1--;
                    saved_register.x0++;
                }
            }

            m_readBufferPos = 0;
            m_mutex.unlock();
        }

        void io::write::handle(kernel::register_maps::general_registers& saved_register) {
            const char* string = (char*)saved_register.x0;

            for (int i = 0; i < saved_register.x1; i++) {
                module::uart::putc(string[i]);
            }

        }

        void io::input_interrupt::handle(kernel::register_maps::general_registers& saved_registers) {
            m_mutex.lock();
            char c = module::uart::getc();

            if (m_readBufferPos < m_readBufferSize) {
                m_readBuffer[m_readBufferPos] = c;
                m_readBufferPos++;
            }

            m_mutex.unlock();
        }

        char io::m_readBuffer[m_readBufferSize + 1];
        int io::m_readBufferPos;
        util::mutex io::m_mutex;
    }
}
