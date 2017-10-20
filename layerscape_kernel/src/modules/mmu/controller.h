#ifndef modules_mmu_controller_h
#define modules_mmu_controller_h

#include "src/kernel/system.h"
#include "src/modules/mmu/table_entry.h"

namespace module {
    namespace mmu {
        class controller {
        public:
            struct info {
                constexpr static const char* title = "MMU Controller";
                constexpr static const bool  multi_load = true;
            };

            static bool load();

            static bool update_va_map_el0(const module::mmu::table_entry* entry, uint16_t asid);
            static uint64_t get_va_map_el0();
            static void set_va_map_el0(const uint64_t value);

            static void enable_mmu();
            static void disable_mmu();
        private:
            static void init_mmu(const module::mmu::table_entry* entry, const module::mmu::table_entry* entry2);

            static void init_mair();

            static table_entry* init_single_table(uint64_t& startAddr);

            static table_entry* kernel_base_entry;
        };
    }
}

#endif
