#ifndef modules_syscall_h
#define modules_syscall_h

#include "src/stddef.h"
#include "src/kernel/system.h"
#include <src/kernel/register_maps.h>
#include "src/modules/exceptions.h"

namespace module {
    namespace syscall {
        // Allows the registration and execution of syscall handlers
        // 
        //  Usage:
        //  struct example_handler {
        //      constexpr static const int id = 30;
        //      static void handle(kernel::register_maps::general_registers& saved_registers);
        //  };
        //    
        //  register_syscall<example_handler>();
        //  
        class controller {
        public:
            struct info {
                constexpr static const char* title = "Syscall Controller";
                constexpr static const bool  multi_load = false;
            };

            constexpr static const int range = 512;

            static bool load();

            template <typename T>
            static bool register_syscall() {
                bool result = false;

                if (range > T::id) {
                    m_syscallTable.handle[T::id] = &T::handle;

                    result = true;
                }

                return result;
            }

            static bool execute(kernel::register_maps::general_registers& saved_registers);
        private:

            struct syscallTable {
                void (*handle[range])(kernel::register_maps::general_registers&);
            };

            static syscallTable m_syscallTable;
        };
    }
}

#endif
