#ifndef modules_syscall_mem_h
#define modules_syscall_mem_h

#include "src/stddef.h"
#include "src/kernel/system.h"
#include "src/kernel/register_maps.h"

namespace module {
    namespace syscall {
        class mem {
        public:
            struct info {
                constexpr static const char* title = "Memory Syscalls";
                constexpr static const bool  multi_load = false;
            };

            static bool load();
        private:
            struct malloc {
                constexpr static const int id = 20;
                static void handle(kernel::register_maps::general_registers& saved_register);
            };

            struct free {
                constexpr static const int id = 21;
                static void handle(kernel::register_maps::general_registers& saved_register);
            };
        };
    }
}

#endif
