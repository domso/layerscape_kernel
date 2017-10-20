#ifndef modules_scheduler_va_map_h
#define modules_scheduler_va_map_h

#include <src/stddef.h>
#include "src/kernel/system.h"
#include "src/modules/mmu/table_entry.h"

namespace module {
    namespace scheduler {
        class va_map {
        public:
            bool init();
            void clear();

            byte* simple_malloc(const uint64_t length, uint64_t attrIndx);
            void simple_free(const byte* start, const uint64_t asid);

            module::mmu::table_entry* get_basetable();
            byte* translate(const byte* VA);
            void print();
        private:

            constexpr static const uint64_t virtualAddressOffset = 0xFFFFFF0000000000;

            bool init_basetable();
            void init_empty_entry(module::mmu::table_entry* currentEntry);
            void clear_table(module::mmu::table_entry* currentTable, int level);
            void set_new_mapping(const uint64_t start, const uint64_t length, uint64_t attrIndx);
            bool check_for_empty_entries(const uint64_t start, const uint64_t length);

            void clear_TLB(const byte* VA, const uint64_t asid);

            module::mmu::table_entry* m_basetable;
        };
    }
}

#endif
