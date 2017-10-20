#ifndef modules_multicore_h
#define modules_multicore_h

#include "src/stddef.h"
#include "src/kernel/system.h"

namespace module {
    class multicore {
    public:
        struct info {
            constexpr static const char* title = "Multicore-Support";
            constexpr static const bool  multi_load = false;
        };

        static bool load();
    private:
        static void wake_up();
        static void start_cpu(byte* stack, byte* spinAddress);
    };
}

#endif
