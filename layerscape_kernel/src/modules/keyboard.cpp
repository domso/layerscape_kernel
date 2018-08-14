#include "src/modules/keyboard.h"

bool module::keyboard::load() {
    
    for (int i = 0; i < 256; i++) {
        asciiBig[i] = '\0';
        asciiLittle[i] = '\0';
    }
    
    asciiBig[0x1E] = 'A';
    asciiBig[0x30] = 'B';
    asciiBig[0x2E] = 'C';
    asciiBig[0x20] = 'D';
    asciiBig[0x12] = 'E';
    asciiBig[0x21] = 'F';
    asciiBig[0x22] = 'G';
    asciiBig[0x23] = 'H';
    asciiBig[0x17] = 'I';
    asciiBig[0x24] = 'J';
    asciiBig[0x25] = 'K';
    asciiBig[0x26] = 'L';
    asciiBig[0x32] = 'M';
    asciiBig[0x31] = 'N';
    asciiBig[0x18] = 'O';
    asciiBig[0x19] = 'P';
    asciiBig[0x10] = 'Q';
    asciiBig[0x13] = 'R';
    asciiBig[0x1F] = 'S';
    asciiBig[0x14] = 'T';
    asciiBig[0x16] = 'U';
    asciiBig[0x2F] = 'V';
    asciiBig[0x11] = 'W';
    asciiBig[0x2D] = 'X';
    asciiBig[0x2C] = 'Y';
    asciiBig[0x15] = 'Z';
    
    asciiBig[0x02] = '!';
    asciiBig[0x03] = '"';
    asciiBig[0x04] = '?';
    asciiBig[0x05] = '$';
    asciiBig[0x06] = '%';
    asciiBig[0x07] = '&';
    asciiBig[0x08] = '/';
    asciiBig[0x09] = '(';
    asciiBig[0x0a] = ')';
    asciiBig[0x0b] = '=';
    asciiBig[0x0c] = '?';
    
    asciiBig[0x39] = ' ';
    asciiBig[0x1C] = '\n';
    
    asciiLittle[0x1E] = 'a';
    asciiLittle[0x30] = 'b';
    asciiLittle[0x2E] = 'c';
    asciiLittle[0x20] = 'd';
    asciiLittle[0x12] = 'e';
    asciiLittle[0x21] = 'f';
    asciiLittle[0x22] = 'g';
    asciiLittle[0x23] = 'h';
    asciiLittle[0x17] = 'i';
    asciiLittle[0x24] = 'j';
    asciiLittle[0x25] = 'k';
    asciiLittle[0x26] = 'l';
    asciiLittle[0x32] = 'm';
    asciiLittle[0x31] = 'n';
    asciiLittle[0x18] = 'o';
    asciiLittle[0x19] = 'p';
    asciiLittle[0x10] = 'q';
    asciiLittle[0x13] = 'r';
    asciiLittle[0x1F] = 's';
    asciiLittle[0x14] = 't';
    asciiLittle[0x16] = 'u';
    asciiLittle[0x2F] = 'v';
    asciiLittle[0x11] = 'w';
    asciiLittle[0x2D] = 'x';
    asciiLittle[0x2C] = 'y';
    asciiLittle[0x15] = 'z';
    
    asciiLittle[0x02] = '1';
    asciiLittle[0x03] = '2';
    asciiLittle[0x04] = '3';
    asciiLittle[0x05] = '4';
    asciiLittle[0x06] = '5';
    asciiLittle[0x07] = '6';
    asciiLittle[0x08] = '7';
    asciiLittle[0x09] = '8';
    asciiLittle[0x0a] = '9';
    asciiLittle[0x0b] = '0';
    asciiLittle[0x0c] = '?';
    
    asciiLittle[0x39] = ' ';
    asciiLittle[0x1C] = '\n';
    
    kernel::system::register_start_application(main);
    return true;
}


void module::keyboard::main() {
    uint8_t key;
    uint8_t old = 0;
    bool enableShift = false;
    
    display::print("\n");
    while (true) {
        asm(
            "in $0x60, %0;"
            : "=r"(key)
        );
        
        if ((key & 0x7F) == 42) {
            enableShift = ((key & 0x80) == 0);
        }
                
        if (key != old && (key & 0x80) == 0) {
            char c;
            if (enableShift) {
                c = asciiBig[key];
            } else {
                c = asciiLittle[key];
            }
            if (c != '\0') {
                display::putc(c);
            }
        }    
        
        old = key;   
    }
}

char module::keyboard::asciiBig[255];
char module::keyboard::asciiLittle[255];
