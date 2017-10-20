#ifndef modules_syscall_thread_h
#define modules_syscall_thread_h

#include "src/stddef.h"
#include "src/kernel/system.h"
#include "src/kernel/register_maps.h"

namespace module {
    namespace syscall {
        class thread {
        public:
            struct info {
                constexpr static const char* title = "Thread Syscalls";
                constexpr static const bool  multi_load = false;
            };

            static bool load();
        private:
            struct clone {
                constexpr static const int id = 220;
                static void handle(kernel::register_maps::general_registers& saved_register);
            };

            struct waitpid {
                constexpr static const int id = 221;
                static void handle(kernel::register_maps::general_registers& saved_register);
            };

            struct detach {
                constexpr static const int id = 222;
                static void handle(kernel::register_maps::general_registers& saved_register);
            };

            struct create {
                constexpr static const int id = 223;
                static void handle(kernel::register_maps::general_registers& saved_register);
            };

            struct exit {
                constexpr static const int id = 93;
                static void handle(kernel::register_maps::general_registers& saved_register);
            };
        };
    }
}

#endif
