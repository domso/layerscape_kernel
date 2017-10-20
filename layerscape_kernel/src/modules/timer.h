#ifndef modulesimer_h
#define modulesimer_h

#include <src/stddef.h>
#include <src/kernel/system.h>
#include <src/kernel/register_maps.h>
#include "src/modules/exceptions.h"

namespace module {
    // Usage:
    // 
    // Step-0: Register handler with the id 'module::timer::interruptID' in the
    // interrupt_handler module.
    // 
    // Step-1: Set timer duration
    //
    // Step-2: Enable timer
    // 
    // 
    class timer {
    public:
        struct info {
            constexpr static const char* title = "Timer";
            constexpr static const bool  multi_load = true;
        };

        constexpr static int interruptID = 30;

        static bool load();

        // done by U.Boot!
        static void set_freq(const int32_t hz);
        static int32_t get_freq();

        static void enable_timer();
        static void disable_timer();

        // increases timer by 'time'/get_freq() seconds
        static void set_timer(const int32_t time);
        // sets timer to 'time'/get_freq() seconds 
        static void set_abs_timer(const int32_t time);
        // increases timer by 'time' seconds
        static void set_timer_sec(const double time);
    private:
        struct default_timer_handler {
            constexpr static const int id = interruptID;
            static void handle(kernel::register_maps::general_registers& saved_registers);
        };
    };
}

#endif
