#include "src/modules/syscall/mem.h"
#include "src/modules/syscall/controller.h"
#include "src/modules/uart.h"

#include "src/modules/scheduler/controller.h"

namespace module {
    namespace syscall {
        bool mem::load() {
            controller::register_syscall<malloc>();
            controller::register_syscall<free>();

            return true;
        }

        void mem::malloc::handle(kernel::register_maps::general_registers& saved_register) {
            auto process = module::scheduler::controller::get_current_process();

            switch (saved_register.x1) {
                case 0: {
                    saved_register.x0 = (uint64_t)process->malloc(saved_register.x0, module::mmu::table_entry::options::attr::ind0);
                    break;
                }

                case 1: {
                    saved_register.x0 = (uint64_t)process->malloc(saved_register.x0, module::mmu::table_entry::options::attr::ind1);
                    break;
                }

                case 2: {
                    saved_register.x0 = (uint64_t)process->malloc(saved_register.x0, module::mmu::table_entry::options::attr::ind2);
                    break;
                }
            }
        }

        void mem::free::handle(kernel::register_maps::general_registers& saved_register) {
            auto process = module::scheduler::controller::get_current_process();
            process->free((byte*)saved_register.x0);
        }
    }
}
