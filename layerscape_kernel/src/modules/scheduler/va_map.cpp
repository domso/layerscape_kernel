#include "src/modules/scheduler/va_map.h"
#include "src/modules/uart.h"
#include "src/modules/mmu/table_entry.h"

namespace module {
    namespace scheduler {
        bool va_map::init() {
            bool result = true;
            result &= init_basetable();

            return result;
        }

        bool va_map::init_basetable() {
            bool result = false;
            m_basetable = (module::mmu::table_entry*)kernel::system::page_alloc();
            module::mmu::table_entry* firstEntry = (module::mmu::table_entry*)kernel::system::page_alloc();

            if (m_basetable != nullptr && firstEntry != nullptr) {
                for (int i = 0; i < kernel::system::page_size() / 8; i++) {
                    m_basetable[i].clear();
                    m_basetable[i].set(module::mmu::table_entry::type, module::mmu::table_entry::options::types::invalid);
                    firstEntry[i].clear();
                    firstEntry[i].set(module::mmu::table_entry::type, module::mmu::table_entry::options::types::invalid);
                }

                m_basetable[0].set(module::mmu::table_entry::type, module::mmu::table_entry::options::types::table);
                m_basetable[0].set(module::mmu::table_entry::addr, (uint64_t)firstEntry);
                result = true;
            }

            return result;
        }

        void va_map::clear() {
            if (m_basetable != nullptr) {
                clear_table(m_basetable, 0);
                kernel::system::page_free((byte*)m_basetable);
                m_basetable = nullptr;
            }
        }

        void va_map::clear_table(mmu::table_entry* currentTable, int level) {
            mmu::table_entry* next;

            for (int i = 0; i < kernel::system::page_size() / 8; i++) {
                if (!currentTable[i].check(module::mmu::table_entry::type, module::mmu::table_entry::options::types::invalid)) {
                    next = (module::mmu::table_entry*) currentTable[i].get(module::mmu::table_entry::addr);

                    if (currentTable[i].check(module::mmu::table_entry::type, module::mmu::table_entry::options::types::table) && level < 3) {
                        clear_table(next, level + 1);
                    }

                    currentTable[i].set(module::mmu::table_entry::type, module::mmu::table_entry::options::types::invalid);
                    kernel::system::page_free((byte*)next);
                }
            }
        }

        byte* va_map::translate(const byte* VA) {
            module::mmu::table_entry* currentTable;
            module::mmu::table_entry* currentEntry;

            uint64_t i = (uint64_t)VA;
            currentTable = (module::mmu::table_entry*) m_basetable[0].get(module::mmu::table_entry::addr);
            currentEntry = &currentTable[(i & 0xFC0000000) >> 30];

            if (currentEntry->check(module::mmu::table_entry::type, module::mmu::table_entry::options::types::invalid)) {
                return (byte*) - 1;
            }

            currentTable = (module::mmu::table_entry*) currentEntry->get(module::mmu::table_entry::addr);
            currentEntry = &currentTable[(i & 0x003FE00000) >> 21];

            if (currentEntry->check(module::mmu::table_entry::type, module::mmu::table_entry::options::types::invalid)) {
                return (byte*) - 1;
            }

            currentTable = (module::mmu::table_entry*) currentEntry->get(module::mmu::table_entry::addr);
            currentEntry = &currentTable[(i & 0x00001FF000) >> 12];

            if (currentEntry->check(module::mmu::table_entry::type, module::mmu::table_entry::options::types::invalid)) {
                return (byte*) - 1;
            }

            return (byte*)currentEntry->get(module::mmu::table_entry::addr);
        }

        void va_map::print() {
            module::mmu::table_entry* currentTable;
            module::mmu::table_entry* currentEntry;

            for (uint64_t i = 0; i < 0x8000000000; i += kernel::system::page_size()) {
                currentTable = (module::mmu::table_entry*) m_basetable[0].get(module::mmu::table_entry::addr);
                currentEntry = &currentTable[(i & 0xFC0000000) >> 30];

                if (!currentEntry->check(module::mmu::table_entry::type, module::mmu::table_entry::options::types::invalid)) {
                    module::uart::printInt<uint64_t>(i);
                    module::uart::print(": table -->");
                    module::uart::printInt<uint64_t>(currentEntry->get(module::mmu::table_entry::addr));
                    module::uart::print("|");

                    currentTable = (module::mmu::table_entry*)currentEntry->get(module::mmu::table_entry::addr);
                    currentEntry = &currentTable[(i & 0x003FE00000) >> 21];

                    if (!currentEntry->check(module::mmu::table_entry::type, module::mmu::table_entry::options::types::invalid)) {
                        module::uart::printInt<uint64_t>(i);
                        module::uart::print(": table -->");
                        module::uart::printInt<uint64_t>(currentEntry->get(module::mmu::table_entry::addr));
                        module::uart::print("|");

                        currentTable = (module::mmu::table_entry*)currentEntry->get(module::mmu::table_entry::addr);
                        currentEntry = &currentTable[(i & 0x00001FF000) >> 12];

                        if (!currentEntry->check(module::mmu::table_entry::type, module::mmu::table_entry::options::types::invalid)) {
                            module::uart::printInt<uint64_t>(i);
                            module::uart::print(": block -->");
                            module::uart::printInt<uint64_t>(currentEntry->get(module::mmu::table_entry::addr));
                            module::uart::print("|");
                        }
                    }

                    module::uart::print("\r\n");
                }
            }
        }

        byte* va_map::simple_malloc(const uint64_t length, uint64_t attrIndx) {
            // 1GB --> 2MB --> 4KB
            uint64_t blockSize = 1;
            uint64_t segSize;

            while (blockSize < length) {
                blockSize = blockSize << 1;
            }            
            
            segSize = length & (~(kernel::system::page_size() - 1));
            
            if ((length & (kernel::system::page_size() - 1)) != 0) {
                segSize += kernel::system::page_size();
            }

            module::mmu::table_entry* currentTable;
            module::mmu::table_entry* currentEntry;

            uint64_t virtualAddress;

            for (uint64_t i = 0; i < 0x8000000000; i += blockSize) {
                uint64_t virtualAddress = i | virtualAddressOffset;

                if (check_for_empty_entries(i, blockSize)) {
                    set_new_mapping(i, segSize, attrIndx);
                    return (byte*)(i | virtualAddressOffset);
                }
            }

            return nullptr;
        }

        void va_map::simple_free(const byte* start, const uint64_t asid) {
            module::mmu::table_entry* currentTable;
            module::mmu::table_entry* currentEntry;
            uint64_t i = (uint64_t)start;

            do {
                currentTable = (module::mmu::table_entry*) m_basetable[0].get(module::mmu::table_entry::addr);
                currentEntry = &currentTable[(i & 0xFC0000000) >> 30];
                init_empty_entry(currentEntry);

                currentTable = (module::mmu::table_entry*) currentEntry->get(module::mmu::table_entry::addr);
                currentEntry = &currentTable[(i & 0x003FE00000) >> 21];
                init_empty_entry(currentEntry);

                currentTable = (module::mmu::table_entry*) currentEntry->get(module::mmu::table_entry::addr);
                currentEntry = &currentTable[(i & 0x00001FF000) >> 12];

                kernel::system::page_free((byte*)currentEntry->get(module::mmu::table_entry::addr));
                currentEntry->set(module::mmu::table_entry::type, module::mmu::table_entry::options::types::invalid);

                clear_TLB((byte*)i, asid);

                i += kernel::system::page_size();
            } while (!currentEntry->check(module::mmu::table_entry::OS, module::mmu::table_entry::options::OS::end_of_block));
        }

        void va_map::init_empty_entry(mmu::table_entry* currentEntry) {
            module::mmu::table_entry tmp;

            if (currentEntry->check(module::mmu::table_entry::type, module::mmu::table_entry::options::types::invalid)) {
                module::mmu::table_entry* newTable = (module::mmu::table_entry*)kernel::system::page_alloc();

                for (int i = 0; i < kernel::system::page_size() / 8; i++) {
                    newTable[i].clear();
                    newTable[i].set(module::mmu::table_entry::type, module::mmu::table_entry::options::types::invalid);
                }


                tmp.clear();
                tmp.set(module::mmu::table_entry::addr, (uint64_t)newTable);
//                 tmp.set(module::mmu::table_entry::APTable, module::mmu::table_entry::options::APTable::rwx_x);
                tmp.set(module::mmu::table_entry::type, module::mmu::table_entry::options::types::table);
                *currentEntry = tmp;
            }
        }

        void va_map::set_new_mapping(const uint64_t start, const uint64_t length, uint64_t attrIndx) {
            module::mmu::table_entry* currentTable;
            module::mmu::table_entry* currentEntry = nullptr;
            module::mmu::table_entry tmp;

            int blockSize = kernel::system::page_size();

            for (uint64_t i = start; i < start + length; i += blockSize) {
                currentTable = (module::mmu::table_entry*) m_basetable[0].get(module::mmu::table_entry::addr);
                currentEntry = &currentTable[(i & 0xFC0000000) >> 30];
                init_empty_entry(currentEntry);

                currentTable = (module::mmu::table_entry*) currentEntry->get(module::mmu::table_entry::addr);
                currentEntry = &currentTable[(i & 0x003FE00000) >> 21];
                init_empty_entry(currentEntry);

                currentTable = (module::mmu::table_entry*) currentEntry->get(module::mmu::table_entry::addr);
                currentEntry = &currentTable[(i & 0x00001FF000) >> 12];

                uint64_t newBlock = (uint64_t)kernel::system::page_alloc();

                tmp.clear();
                tmp.set(module::mmu::table_entry::addr, newBlock);     //newBlock
                tmp.set(module::mmu::table_entry::type, module::mmu::table_entry::options::types::block_4KB);
                tmp.set(module::mmu::table_entry::AP, module::mmu::table_entry::options::AP::rw_rwx);
                tmp.set(module::mmu::table_entry::SH, module::mmu::table_entry::options::SH::inner_shareable);
                tmp.enable(module::mmu::table_entry::AF);
                tmp.enable(module::mmu::table_entry::NG);

                tmp.set(module::mmu::table_entry::SH, module::mmu::table_entry::options::SH::outer_shareable);
                tmp.set(module::mmu::table_entry::attrIndx, attrIndx);

                *currentEntry = tmp;
            }

            if (currentEntry != nullptr) {
                currentEntry->set(module::mmu::table_entry::OS, module::mmu::table_entry::options::OS::end_of_block);
            }

        }

        bool va_map::check_for_empty_entries(const uint64_t start, const uint64_t length) {
            module::mmu::table_entry* currentTable;
            module::mmu::table_entry* currentEntry;

            for (uint64_t i = start; i < start + length; i += kernel::system::page_size()) {
                currentTable = (module::mmu::table_entry*) m_basetable[0].get(module::mmu::table_entry::addr);
                currentEntry = &currentTable[(i & 0xFC0000000) >> 30];
                init_empty_entry(currentEntry);

                currentTable = (module::mmu::table_entry*) currentEntry->get(module::mmu::table_entry::addr);
                currentEntry = &currentTable[(i & 0x003FE00000) >> 21];
                init_empty_entry(currentEntry);

                currentTable = (module::mmu::table_entry*) currentEntry->get(module::mmu::table_entry::addr);
                currentEntry = &currentTable[(i & 0x00001FF000) >> 12];

                if (!currentEntry->check(module::mmu::table_entry::type, module::mmu::table_entry::options::types::invalid)) {
                    return false;
                }
            }

            return true;
        }

        mmu::table_entry* va_map::get_basetable() {
            return m_basetable;
        }

        void va_map::clear_TLB(const byte* VA, const uint64_t asid) {
            asm(
                "lsl    x1, x1, 48\n"
                "and    x0, x0, #0xFFFFFFFFFFFF\n"
                "orr    x0, x0, x1\n"
                "TLBI VAE1, x0\n"
                "ISB\n"
                "DSB sy\n"
            );
        }
    }
}
