#include "src/modules/mmu/cache.h"

namespace module {
    namespace mmu {
        void cache::invalidatelb() {
            asm(
                "tlbi    alle3\n"
                "dsb sy\n"
                "isb\n"
            );
        }

        // this code was taken from "u-boot/arch/arm/cpu/armv8/cache.S"
        // --> https://github.com/u-boot/u-boot/blob/master/arch/arm/cpu/armv8/cache.S
        void cache::invalidate_data() {
            asm(
                "mov  x0, #0\n"
                "__asm_flush_dcache_level:\n"
                "lsl x12, x0, #1\n"
                "msr csselr_el1, x12     /* select cache level */\n"
                "isb             /* sync change of cssidr_el1 */\n"
                "mrs x6, ccsidr_el1      /* read the new cssidr_el1 */\n"
                "and x2, x6, #7      /* x2 <- log2(cache line size)-4 */\n"
                "add x2, x2, #4      /* x2 <- log2(cache line size) */\n"
                "mov x3, #0x3ff\n"
                "and x3, x3, x6, lsr #3  /* x3 <- max number of #ways */\n"
                "clz w5, w3          /* bit position of #ways */\n"
                "mov x4, #0x7fff\n"
                "and x4, x4, x6, lsr #13 /* x4 <- max number of #sets */\n"
                "/* x12 <- cache level << 1 */\n"
                "/* x2 <- line length offset */\n"
                "/* x3 <- number of cache ways - 1 */\n"
                "/* x4 <- number of cache sets - 1 */\n"
                "/* x5 <- bit position of #ways */\n"

                "loop_set:\n"
                "mov x6, x3          /* x6 <- working copy of #ways */\n"
                "loop_way:\n"
                "lsl x7, x6, x5\n"
                "orr x9, x12, x7     /* map way and level to cisw value */\n"
                "lsl x7, x4, x2\n"
                "orr x9, x9, x7      /* map set number to cisw value */\n"
                "tbz w1, #0, 1f\n"
                "dc  isw, x9\n"
                "b   2f\n"
                "1:  dc  cisw, x9        /* clean & invalidate by set/way */\n"
                "2:  subs    x6, x6, #1      /* decrement the way */\n"
                "b.ge    loop_way\n"
                "subs    x4, x4, #1      /* decrement the set */\n"
                "b.ge    loop_set\n"

                "ret     \n"

                "__asm_dcache_all:\n"
                "mov x1, x0\n"
                "dsb sy\n"
                "mrs x10, clidr_el1      /* read clidr_el1 */\n"
                "lsr x11, x10, #24\n"
                "and x11, x11, #0x7      /* x11 <- loc */\n"
                "cbz x11, finished       /* if loc is 0, exit */\n"
                "mov x15, x30\n"
                "mov x0, #0          /* start flush at cache level 0 */\n"
                "/* x0  <- cache level */\n"
                "/* x10 <- clidr_el1 */\n"
                "/* x11 <- loc */\n"
                "/* x15 <- return address */\n"

                "loop_level:\n"
                "lsl x12, x0, #1\n"
                "add x12, x12, x0        /* x0 <- tripled cache level */\n"
                "lsr x12, x10, x12\n"
                "and x12, x12, #7        /* x12 <- cache type */\n"
                "cmp x12, #2\n"
                "b.lt    skip            /* skip if no cache or icache */\n"
                "bl  __asm_flush_dcache_level    /* x1 = 0 flush, 1 invalidate */\n"
                "skip:\n"
                "add x0, x0, #1      /* increment cache level */\n"
                "cmp x11, x0\n"
                "b.gt    loop_level\n"

                "mov x0, #0\n"
                "msr csselr_el1, x0      /* restore csselr_el1 */\n"
                "dsb sy\n"
                "isb\n"
                "mov x30, x15\n"

                "finished:\n"
                "ret\n"
            );
        }
    }
}
