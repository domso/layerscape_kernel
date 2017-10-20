#ifndef modules_hang_h
#define modules_hang_h

#include "src/stddef.h"
#include "src/kernel/system.h"

namespace module {
    // Sets the initial application to a simple infinite loop with dotted output
    class hang {
    public:
        struct info {
            constexpr static const char* title = "Hang";
            constexpr static const bool  multi_load = false;
        };

        static bool load();
    private:
        static void loop();
    };
}

#endif

