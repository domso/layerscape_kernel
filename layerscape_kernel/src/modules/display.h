#ifndef modules_display_h
#define modules_display_h

#include "src/stddef.h"
#include "src/kernel/system.h"

namespace module {
class display {
public:
    struct info {
        constexpr static const char* title = "Display";
        constexpr static const bool  multi_load = false;
    };

    static bool load();
    static void clear();
    static void print(const char* string);
    static void putc(const char c);

    template <typename T>
    static void printInt(T c) {
        int32_t m = 1;
        char out;
        
        while (m * 10 <= c) {
            m *= 10;
        }

        while (0 < m) {
            out = 48;

            while (m <= c) {
                out++;
                c -= m;
            }

            putc(out);
            m /= 10;
        }
    }
    
    static void remc();
private:
    constexpr static const intptr_t vidptr = 0xb8000;
    constexpr static const int width = 80;
    constexpr static const int height = 25;

    static void scroll_down();

    static uint32_t cursor_position;
};
}

#endif


