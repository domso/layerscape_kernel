#ifndef modules_syscall_performance_h
#define modules_syscall_performance_h

#include "src/stddef.h"
#include "src/kernel/system.h"
#include "src/kernel/register_maps.h"

namespace module {
    namespace syscall {
        class performance {
        public:
            struct info {
                constexpr static const char* title = "Performance Syscalls";
                constexpr static const bool  multi_load = true;
            };

            static bool load();
        private:
            struct instr_retired {
                constexpr static const int id = 85;
                static void handle(kernel::register_maps::general_registers& saved_register);
            };
        };
    }
}

#endif
