#include "src/modules/mmu/mair.h"

namespace module {
    namespace mmu {
        void mair::set(const uint8_t attr, const int attrIndex) {
            uint64_t setMask = attr;
            uint64_t clearMask = 0xFF;
            uint64_t current;

            setMask = setMask << (attrIndex * 8);
            clearMask = clearMask << (attrIndex * 8);

            current = get_mair();
            current = (current & (~clearMask)) | setMask;

            set_mair(current);
        }

        void mair::clear() {
            set_mair(0);
        }

        void mair::set_mair(const uint64_t attr) {
            asm(
                "msr    MAIR_EL1, x0\n"
                "dsb    sy\n"
                "isb\n"
            );
        }

        uint64_t mair::get_mair() {
            asm("mrs    x0, MAIR_EL1");
        }
    }
}
