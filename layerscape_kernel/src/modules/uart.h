#ifndef modules_uart_h
#define modules_uart_h

#include <src/stddef.h>
#include "src/kernel/system.h"
#include "src/kernel/register_maps.h"
#include "src/modules/exceptions.h"
#include "src/util/mutex.h"

namespace module {
    // Simple UART driver with lowlevel support
    // for printing texts and numbers to the serial console
    class uart {
    public:
        struct info {
            constexpr static const char* title = "Serial UART";
            constexpr static const bool  multi_load = false;
        };

        static bool load();

        static void print(const char* string);
        static void println(const char* string);

        static char getc();
        static void putc(const char c);

        //very experimental
        template <typename T>
        static void printInt(T c) {
            int64_t m = 1;
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

        static void printHex(uint64_t c) {
            char output[19];
            output[18] = '\0';
            output[0] = '0';
            output[1] = 'x';
            int i = 2;

            for (; i < 18; i++) {
                output[i] = '0';
            }

            i = 17;

            while (c > 0) {
                output[i] += c & 0xF;

                if (output[i] > 57) {
                    output[i] += 7;
                }

                c = c >> 4;
                i--;
            }

            print(output);
        }

    private:
        struct registers {
            volatile uint8_t THR_RBR_DLL;       //Transmitter Holding Buffer |  Receiver Buffer | Divisor Latch Low Byte
            volatile uint8_t IER_DLH;           //Interrupt Enable Register | Divisor Latch High Byte
            volatile uint8_t IIR_FCR;           //Interrupt Identification Register | FIFO Control Register
            volatile uint8_t LCR;               //Line Control Register
            volatile uint8_t MCR;               //Modem Control Register
            volatile const uint8_t LSR;         //Line Status Register
            volatile const uint8_t MSR;         //Modem Status Register
            volatile uint8_t SR;                //Scratch Register
        };

        constexpr static volatile registers* PORT0 = (volatile registers*)0x21c0500;
        constexpr static volatile registers* PORT1 = (volatile registers*)0x21c0600;

        struct input_interrupt {
            constexpr static const int id = 64;
            static void handle(kernel::register_maps::general_registers& saved_registers);
        };

        static void enable_interrupts();
        static util::mutex m_mutex;
    };
}

#endif
