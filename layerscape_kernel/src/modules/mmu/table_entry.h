#ifndef modules_mmuable_entry_h
#define modules_mmuable_entry_h

#include <src/stddef.h>

namespace module {
    namespace mmu {
        class table_entry {
        public:

            struct options {
                struct types {
                    constexpr static const uint64_t invalid         =  0x0;
                    constexpr static const uint64_t table           =  0x3;
                    constexpr static const uint64_t block_1GB       =  0x1;
                    constexpr static const uint64_t block_2MB       =  0x1;
                    constexpr static const uint64_t block_4KB       =  0x3;
                };

                struct attr {
                    constexpr static const uint64_t ind0            =  0x00;
                    constexpr static const uint64_t ind1            =  0x04;
                    constexpr static const uint64_t ind2            =  0x08;
                    constexpr static const uint64_t ind3            =  0x0C;
                    constexpr static const uint64_t ind4            =  0x10;
                    constexpr static const uint64_t ind5            =  0x14;
                    constexpr static const uint64_t ind6            =  0x18;
                    constexpr static const uint64_t ind7            =  0x1C;
                };

                struct AP {
                    // D4-34 / D4-2077
                    // r: read
                    // w: write
                    // x: execute (wird von UXN, PXN überschrieben)
                    //
                    // <EL1>_<EL0>
                    constexpr static const uint64_t rwx_x           = 0x00;
                    constexpr static const uint64_t rw_rwx          = 0x40;
                    constexpr static const uint64_t rx_x            = 0x80;
                    constexpr static const uint64_t rx_rx           = 0xC0;
                };

                struct APTable {
                    // D4-2075
                    // r: read
                    // w: write
                    // x: execute (wird von UXN, PXN überschrieben)
                    //
                    // <EL1>_<EL0>
                    constexpr static const uint64_t rwx_rwx         = 0x0000000000000000;
                    constexpr static const uint64_t rwx             = 0x2000000000000000;
                    constexpr static const uint64_t rx_rx           = 0x4000000000000000;
                    constexpr static const uint64_t rx              = 0x6000000000000000;
                };

                struct SH {
                    constexpr static const uint64_t non_shareable   = 0x000;
                    constexpr static const uint64_t reserved        = 0x100;
                    constexpr static const uint64_t outer_shareable = 0x200;
                    constexpr static const uint64_t inner_shareable = 0x300;
                };

                // Software-defined
                struct OS {
                    constexpr static const uint64_t end_of_block    = 0x0080000000000000;
                };
            };

            // masks:
            constexpr static const uint64_t all             =  0xFFFFFFFFFFFFFFFF;
            constexpr static const uint64_t addr            =  0x000000FFFFFFF000;

            // Table
            constexpr static const uint64_t NSTable         =  0x8000000000000000;
            constexpr static const uint64_t APTable         =  0x6000000000000000;
            constexpr static const uint64_t UXNTable        =  0x1000000000000000;
            constexpr static const uint64_t PXNTable        =  0x0800000000000000;


            constexpr static const uint64_t OS              =  0x0780000000000000;
            constexpr static const uint64_t UXN             =  0x0040000000000000;
            constexpr static const uint64_t PXN             =  0x0020000000000000;
            constexpr static const uint64_t CON             =  0x0010000000000000;

            constexpr static const uint64_t NG              =  0x0000000000000800;
            constexpr static const uint64_t AF              =  0x0000000000000400;
            constexpr static const uint64_t SH              =  0x0000000000000300;
            constexpr static const uint64_t AP              =  0x00000000000000C0;
            constexpr static const uint64_t NS              =  0x0000000000000020;
            constexpr static const uint64_t attrIndx        =  0x000000000000001C;
            constexpr static const uint64_t type            =  0x0000000000000003;


            void set(const uint64_t mask, const uint64_t options);

            void enable(const uint64_t mask);

            void disable(const uint64_t mask);

            bool check(const uint64_t mask, const uint64_t options);

            uint64_t get(const uint64_t mask);

            void clear();
        private:
            uint64_t m_fields;
        };
    }
}

#endif
