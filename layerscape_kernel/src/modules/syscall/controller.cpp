#include "src/modules/syscall/controller.h"
#include "src/modules/uart.h"

namespace module {
    namespace syscall {
        bool controller::load() {
            for (int i = 0; i < range; i++) {
                m_syscallTable.handle[i] = nullptr;
            }

            return true;
        }

        bool controller::execute(kernel::register_maps::general_registers& saved_registers) {
            bool result = false;
            int syscallNumber = -1;
            syscallNumber = saved_registers.x8;

            if (0 <= syscallNumber && syscallNumber < range) {
                if (m_syscallTable.handle[syscallNumber] != nullptr) {
                    m_syscallTable.handle[syscallNumber](saved_registers);
                    result = true;
                }
            }

            return result;
        }

        controller::syscallTable controller::m_syscallTable;
    }
}
