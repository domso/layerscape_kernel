#include "src/modules/syscall/thread.h"
#include "src/modules/syscall/controller.h"
#include "src/modules/scheduler/controller.h"

namespace module {
    namespace syscall {
        bool thread::load() {
            controller::register_syscall<clone>();
            controller::register_syscall<waitpid>();
            controller::register_syscall<detach>();
            controller::register_syscall<create>();
            controller::register_syscall<exit>();

            return true;
        }

        void thread::clone::handle(kernel::register_maps::general_registers& saved_register) {
            auto process = module::scheduler::controller::get_current_process();
            process->store_context(saved_register);
            saved_register.x0 = module::scheduler::controller::clone_process(*process, (byte*) saved_register.x1, (module::scheduler::process::attributes*) saved_register.x2);
        }

        void thread::waitpid::handle(kernel::register_maps::general_registers& saved_register) {
            auto process = module::scheduler::controller::get_current_process();
            module::scheduler::controller::wait_for(saved_register.x0, *process);
            module::scheduler::controller::context_switch(saved_register);
        }

        void thread::detach::handle(kernel::register_maps::general_registers& saved_register) {
            module::scheduler::controller::context_switch(saved_register);
        }

        void thread::create::handle(kernel::register_maps::general_registers& saved_register) {
            auto proc = scheduler::controller::create_process((byte*)saved_register.x0, saved_register.x1, (byte*)saved_register.x2);
            saved_register.x0 = (uint64_t)proc->config().id;
        }

        void thread::exit::handle(kernel::register_maps::general_registers& saved_register) {
            auto process = module::scheduler::controller::get_current_process();
            process->signal(module::scheduler::process_status::signals::SIGTERM);
            module::scheduler::controller::context_switch(saved_register);
        }
    }
}
