#ifndef modules_keyboard_h
#define modules_keyboard_h

#include "src/stddef.h"
#include "src/kernel/system.h"
#include "src/modules/display.h"

namespace module {
class keyboard {
public:
    struct info {
        constexpr static const char* title = "Keyboard";
        constexpr static const bool  multi_load = false;
    };

    static bool load();    
private:
    static void main();
    static char asciiBig[255];
    static char asciiLittle[255];
};
}

#endif




