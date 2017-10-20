#ifndef modules_exception_h
#define modules_exception_h

#include <src/stddef.h>
#include "src/kernel/system.h"

namespace module {
    class exceptions {
    public:
        struct info {
            constexpr static const char* title = "Exceptions";
            constexpr static const bool  multi_load = false;
        };

        static bool load();

        static void error_msg(byte* sp);
        static void interrupt(byte* sp);
    };
}

#endif





















