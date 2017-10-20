#include <stdint.h>
#include "syscalls.h"

#include "string.h"
#include "apps.h"

struct options {
    constexpr static const uint64_t inputBufferSize = 100;
    constexpr static const uint64_t commandLineBufferSize = 1024;
};

void command(const char* cmd, const int length);

// force a fixed entry-point
int main() __attribute__((section(".start")));

int main() {
    app::init();
    app_list();       
    
    int numRecvChar;
    char inputBuffer[options::inputBufferSize];

    char commandLineBuffer[options::commandLineBufferSize + 1];
    
    int startInputInLine = 4;
    int currentPos = startInputInLine;
    int currentSize = startInputInLine;

    commandLineBuffer[0] = '\r';
    commandLineBuffer[1] = '$';
    commandLineBuffer[2] = ':';
    commandLineBuffer[3] = '>';

    write(commandLineBuffer, currentSize + 1);

    while (1) {
        numRecvChar = read(inputBuffer, options::inputBufferSize);

        if (numRecvChar != 0) {
            for (int i = 0; i < numRecvChar; i++) {
                if (currentPos < options::commandLineBufferSize) {

                    switch (inputBuffer[i]) {
                        case '\r': {
                            commandLineBuffer[currentPos] = '\r';
                            commandLineBuffer[currentPos + 1] = '\n';
                            write(commandLineBuffer, currentSize + 2);
                            command(commandLineBuffer + startInputInLine, currentSize - startInputInLine);

                            for (int i = startInputInLine; i < currentSize; i++) {
                                commandLineBuffer[i] = ' ';
                            }

                            write(commandLineBuffer, currentSize);
                            currentSize = startInputInLine;
                            currentPos = startInputInLine;
                            break;
                        }

                        case 127: {
                            currentPos--;

                            if (currentPos < startInputInLine) {
                                currentPos = startInputInLine;
                            } else {
                                commandLineBuffer[currentPos] = ' ';
                                write(commandLineBuffer, currentSize);

                                if (currentPos + 1 == currentSize) {
                                    currentSize--;
                                }
                            }

                            break;
                        }

                        default: {
                            commandLineBuffer[currentPos] = inputBuffer[i];
                            currentPos++;
                            currentSize++;
                            break;
                        }
                    }
                }
            }

            write(commandLineBuffer, currentSize);
        }
    }

    return 0;
}

void command(const char* cmd, const int length) {
    if (length != 0) {
        int cmdLength;

        for (cmdLength = 0; cmdLength < length && cmd[cmdLength] != ' '; cmdLength++) {}

        string exec(cmd, cmdLength);
        string args(cmd + cmdLength, length - cmdLength);
        
        if (!app::start_exec(exec)) {
            write("Command not found", 17);            
            write("\r\n", 2);
        }
    }
}
