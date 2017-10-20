#ifndef modules_syscall_io_h
#define modules_syscall_io_h

#include "src/stddef.h"
#include "src/kernel/system.h"
#include "src/kernel/register_maps.h"
#include "src/util/mutex.h"

namespace module {
    namespace syscall {
        class io {
        public:
            struct info {
                constexpr static const char* title = "IO Syscalls";
                constexpr static const bool  multi_load = false;
            };

            static bool load();
        private:
            struct read {
                constexpr static const int id = 63;
                static void handle(kernel::register_maps::general_registers& saved_register);
            };

            struct write {
                constexpr static const int id = 64;
                static void handle(kernel::register_maps::general_registers& saved_register);
            };

            struct input_interrupt {
                constexpr static const int id = 64;
                static void handle(kernel::register_maps::general_registers& saved_registers);
            };

            constexpr static int m_readBufferSize = 10;
            static char m_readBuffer[m_readBufferSize + 1];
            static int m_readBufferPos;
            static util::mutex m_mutex;
        };
    }
}

#endif
