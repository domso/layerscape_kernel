#ifndef modules_interrupt_handler_h
#define modules_interrupt_handler_h

#include "src/stddef.h"
#include "src/kernel/system.h"
#include <src/kernel/register_maps.h>
#include "src/modules/exceptions.h"

namespace module {
    // Allows the registration and execution of interrupt handlers
    // 
    //  Usage:
    //  struct example_handler {
    //      constexpr static const int id = 30;
    //      static void handle(kernel::register_maps::general_registers& saved_registers);
    //  };
    //    
    //  register_interrupt_handler<example_handler>();
    //  
    class interrupt_handler {
    public:
        struct info {
            constexpr static const char* title = "Interrupt Handler";
            constexpr static const bool  multi_load = false;
        };

        constexpr static const int range = 1024;

        static bool load();

        template <typename T>
        static bool register_interrupt_handler() {
            bool result = false;

            if (range > T::id) {
                m_interruptHandlerTable.handle[T::id] = &T::handle;

                result = true;
            }

            return result;
        }

        static bool execute(const int interrupt_handler_number, kernel::register_maps::general_registers& saved_registers);

    private:
        struct interrupt_handlerable {
            void (*handle[range])(kernel::register_maps::general_registers&);
        };

        static interrupt_handlerable m_interruptHandlerTable;
    };
}
#endif
