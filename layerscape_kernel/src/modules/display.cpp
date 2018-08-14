#include "src/modules/display.h"

namespace module {
bool display::load() {
    clear();
    kernel::system::register_stdout(&print);

    return true;
}

void display::clear() {
    volatile char* ptr = (char*) vidptr;
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        ptr[i] = ' ';
        ptr[i + 1] = 0x07;
    }

    cursor_position = 0;
}

void display::print(const char* string) {
    int i;
    for (i = 0; string[i] != '\0'; i++ ) {
        putc(string[i]);
    }
}

void display::putc(const char c) {    
    volatile char* ptr = (char*) vidptr;
    if (c == '\n') {
        cursor_position -= (cursor_position % width);
        cursor_position += width;
    } else {
        ptr[cursor_position * 2] = c;
        ptr[cursor_position * 2 + 1] = 0x07;
        cursor_position++;
    }
    
    if ((cursor_position % width) == 0 && (cursor_position / width) == height) {                
        scroll_down();
        cursor_position -= width;
    }
}

void display::scroll_down() {
    volatile char* ptr = (char*) vidptr;
    int i;
    for (i = 0; i < (height - 1) * width * 2; i += 2) {
        ptr[i] = ptr[i + width * 2];
        ptr[i + 1] = ptr[i + width * 2 + 1];
    }
    
    for (; i < height * width * 2; i += 2) {
        ptr[i] = ' ';
        ptr[i + 1] = 0x07;
    }
}


uint32_t display::cursor_position;

}

