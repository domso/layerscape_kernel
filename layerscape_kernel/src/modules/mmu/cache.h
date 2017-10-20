#ifndef modules_mmu_cache_h
#define modules_mmu_cache_h

namespace module {
    namespace mmu {
        class cache {
        public:
            static void invalidatelb();

            // this code was taken from "u-boot/arch/arm/cpu/armv8/cache.S"
            // --> https://github.com/mripard/u-boot/blob/bd328eb38274ffaf04caaa8a6ecc09b7e19a650e/arch/arm/cpu/armv8/cache.S
            static void invalidate_data();
        };
    }
}
#endif
