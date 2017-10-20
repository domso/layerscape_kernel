#include "src/modules/mmu/controller.h"
#include "src/modules/mmu/table_entry.h"
#include "src/modules/mmu/cache.h"
#include "src/modules/mmu/mair.h"
#include <src/modules/uart.h>

namespace module {
    namespace mmu {
        bool controller::load() {
            table_entry* lvl0Entries;

            if (kernel::system::is_core0()) {
                if (kernel::system::page_size() != 4096) {
                    // wrong page size
                    return false;
                }

                lvl0Entries = (table_entry*)kernel::system::page_alloc();

                uint64_t startAddr = 0;
                kernel_base_entry = lvl0Entries;

                for (int i = 0; i < 512; i++) {
                    lvl0Entries[i].clear();
                }

                lvl0Entries[0].set(table_entry::type, table_entry::options::types::table);
                lvl0Entries[0].set(table_entry::addr, (uint64_t)init_single_table(startAddr));

                lvl0Entries[1].set(table_entry::type, table_entry::options::types::table);
                lvl0Entries[1].set(table_entry::addr, (uint64_t)init_single_table(startAddr));

            } else {
                lvl0Entries = kernel_base_entry;
            }

            // cache is invalidated in startup.S

            init_mair();
            init_mmu(lvl0Entries, lvl0Entries);
            enable_mmu();

            return true;
        }

        table_entry* controller::init_single_table(uint64_t& startAddr) {
            table_entry* table = (table_entry*)kernel::system::page_alloc();

            for (int i = 0; i < 512; i++) {
                table[i].clear();
                table[i].set(table_entry::type, table_entry::options::types::block_1GB);
                table[i].set(table_entry::AP, table_entry::options::AP::rwx_x);
                table[i].set(table_entry::SH, table_entry::options::SH::inner_shareable);
                table[i].enable(table_entry::AF);
                table[i].enable(table_entry::NG);

                table[i].set(table_entry::SH, table_entry::options::SH::outer_shareable);

                table[i].set(table_entry::addr, startAddr);

                if (startAddr < 0x80000000) {
                    table[i].set(table_entry::attrIndx, table_entry::options::attr::ind0);
                    table[i].enable(table_entry::UXN);
                    table[i].enable(table_entry::PXN);
                } else {
                    table[i].set(table_entry::attrIndx, table_entry::options::attr::ind1);
                }

                startAddr += 0x40000000;
            }

            return table;
        }

        bool controller::update_va_map_el0(const module::mmu::table_entry* entry, uint16_t asid) {
            asm(
                "lsl    x1, x1, #48\n"
                "orr    x0, x0, x1\n"

                "msr    TTBR1_EL1, x0\n"
                "dsb    sy\n"
                "isb\n"
                "TLBI   ASIDE1, x1\n"
                "dsb    sy\n"
                "isb\n"
            );
        }

        uint64_t controller::get_va_map_el0() {
            asm("mrs    x0, TTBR1_EL1");
        }

        void controller::set_va_map_el0(const uint64_t value) {
            asm(
                "msr    TTBR1_EL1, x0\n"
                "dsb    sy\n"
                "isb\n"
                "TLBI   ASIDE1, x1\n"
                "dsb    sy\n"
                "isb\n"
            );
        }

        void controller::init_mmu(const table_entry* entry, const table_entry* entry2) {
            asm(
                "msr    TTBR0_EL1, x0\n"
                "msr    TTBR1_EL1, x1\n"
                "ldr    x0, =0x12B5583518\n"
                "msr    TCR_EL1, x0\n"
                "dsb    sy\n"
                "isb\n"
            );
        }

        void controller::enable_mmu() {
            asm(
                "mrs    x9, SCTLR_EL1\n"
//                 "orr    x9, x9, #(0x1 << 2)\n"  // enable icache
//                 "orr    x9, x9, #(0x1 << 12)\n" // enable dcache
                "orr    x9, x9, #0x1\n"         // enable MMU
                "msr    SCTLR_EL1, x9\n"
                "dsb    sy\n"
                "isb\n"
            );
        }

        //NOTE: BE VERY CAREFULL! AFTER THIS FUNCTION THE STACK MAY BE CORRUPTED!!
        void controller::disable_mmu() {
            asm(
                "mrs    x9, SCTLR_EL1\n"
//                 "eor    x9, x9, #(0x1 << 2)\n"  // disable icache
//                 "eor    x9, x9, #(0x1 << 12)\n" // disable dcache
                "eor    x9, x9, #0x1\n"         // disable MMU
                "msr    SCTLR_EL1, x9\n"
                "dsb    sy\n"
                "isb\n"
            );
        }

        void controller::init_mair() {
            module::mmu::mair::clear();
            module::mmu::mair::set(module::mmu::mair::device_attr::device_memory | module::mmu::mair::device_attr::nGnRnE, 0);

            module::mmu::mair::set(module::mmu::mair::normal_memory_attr::outer_read_allocate |
                                   module::mmu::mair::normal_memory_attr::outer_write_allocate |
                                   module::mmu::mair::normal_memory_attr::outer_write_back |
                                   module::mmu::mair::normal_memory_attr::outer_non_transient |

                                   module::mmu::mair::normal_memory_attr::inner_read_allocate |
                                   module::mmu::mair::normal_memory_attr::inner_write_allocate |
                                   module::mmu::mair::normal_memory_attr::inner_write_back |
                                   module::mmu::mair::normal_memory_attr::inner_non_transient
                                   , 1);

            module::mmu::mair::set(module::mmu::mair::normal_memory_attr::outer_non_cacheable | module::mmu::mair::normal_memory_attr::inner_non_cacheable, 2);

            module::mmu::mair::set(module::mmu::mair::normal_memory_attr::outer_read_allocate |
                                   module::mmu::mair::normal_memory_attr::outer_write_allocate |
                                   module::mmu::mair::normal_memory_attr::outer_non_transient |

                                   module::mmu::mair::normal_memory_attr::inner_read_allocate |
                                   module::mmu::mair::normal_memory_attr::inner_write_allocate |
                                   module::mmu::mair::normal_memory_attr::inner_non_transient
                                   , 3);
        }

        table_entry* controller::kernel_base_entry;
    }
}
