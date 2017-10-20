#ifndef modules_mmu_mair_h
#define modules_mmu_mair_h

#include <src/stddef.h>

namespace module {
    namespace mmu {
        // see MAIR_EL1 in the ARM-Documentation
        class mair {
        public:

            struct device_attr {
                constexpr static const uint8_t device_memory                      = 0x00;
                constexpr static const uint8_t nGnRnE                             = 0x00;
                constexpr static const uint8_t nGnRE                              = 0x04;
                constexpr static const uint8_t nGRE                               = 0x08;
                constexpr static const uint8_t GRE                                = 0x0C;
            };

            struct normal_memory_attr {
                constexpr static const uint8_t outer_write_allocate               = 0x10;
                constexpr static const uint8_t outer_read_allocate                = 0x20;
                constexpr static const uint8_t outer_write_back                   = 0x40;
                constexpr static const uint8_t outer_non_cacheable                = 0x40;
                constexpr static const uint8_t outer_non_transient                = 0x80;

                constexpr static const uint8_t inner_write_allocate               = 0x01;
                constexpr static const uint8_t inner_read_allocate                = 0x02;
                constexpr static const uint8_t inner_write_back                   = 0x04;
                constexpr static const uint8_t inner_non_cacheable                = 0x04;
                constexpr static const uint8_t inner_non_transient                = 0x08;
            };

            static void set(const uint8_t attr, const int attrIndex);
            static void clear();
        private:
            static void set_mair(const uint64_t attr);
            static uint64_t get_mair();
        };
    }
}

#endif
