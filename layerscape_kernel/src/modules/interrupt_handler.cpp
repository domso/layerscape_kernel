#include "src/modules/interrupt_handler.h"
#include "src/modules/uart.h"
#include "src/modules/timer.h"

namespace module {
    bool interrupt_handler::load() {
        for (int i = 0; i < range; i++) {
            m_interruptHandlerTable.handle[i] = nullptr;
        }

        return true;
    }

    bool interrupt_handler::execute(const int interrupt_handler_number, kernel::register_maps::general_registers& saved_registers) {
        bool result = false;

        if (0 <= interrupt_handler_number && interrupt_handler_number < range) {
            if (m_interruptHandlerTable.handle[interrupt_handler_number] != nullptr) {
                m_interruptHandlerTable.handle[interrupt_handler_number](saved_registers);
                result = true;
            }
        }

        return result;
    }

    interrupt_handler::interrupt_handlerable interrupt_handler::m_interruptHandlerTable;
}



