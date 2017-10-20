.globl init_el3
.globl init_el2
#---------------------------------------------------------------------------------------
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Init-EL3~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#---------------------------------------------------------------------------------------
# primary core starts here
init_el3:
    ldr x19, =stack_top
    mov x20, xzr
#---------------------------------------------------------------------------------------   
disable_caches_and_MMU_EL3:
    msr SCTLR_EL3, xzr 
    isb 
#---------------------------------------------------------------------------------------   
disable_NEON_FP_traps_EL3:
    msr CPTR_EL3, xzr
#---------------------------------------------------------------------------------------        
set_new_exception_vector_EL3:
    ldr   x0,=exception_vector
    msr   VBAR_EL3, x0
#---------------------------------------------------------------------------------------
init_exception_vector:
    ldr  x0, =hang
    
    ldr  x1, =exception_sync
    str  x0, [x1]
    
    ldr  x1, =exception_SError
    str  x0, [x1]
    
    ldr  x0, =ignore
    
    ldr  x1, =exception_IRQ
    str  x0, [x1]
    
    ldr  x1, =exception_FIQ
    str  x0, [x1]    
#---------------------------------------------------------------------------------------
flush_dcache_and_tlb:
    bl cache_flush  
#---------------------------------------------------------------------------------------
prepare_switch_from_EL3_to_EL1:
    mov x0, #0
    orr x0, x0, #(1<<10) // RW
    orr x0, x0, #(1<<0) // NS      
    msr    SCR_EL3, x0
    
    mov x0, #0b0101 
    msr SPSR_EL3, x0
    
    adr x0, el1_entry
    msr ELR_EL3, x0 
    
    msr daifset, #0xF;
#---------------------------------------------------------------------------------------
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Init-EL2~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#---------------------------------------------------------------------------------------
# secondary cores start here
init_el2:
    //x19 should contain SP
    //x20 should contain not zero
#---------------------------------------------------------------------------------------   
disable_caches_and_MMU_EL2_1_0:
    msr SCTLR_EL2, xzr 
    mov x0, #0x50000    //enable wfi and wfe
    msr SCTLR_EL1, x0
    msr HCR_EL2, xzr
    isb 
#---------------------------------------------------------------------------------------
disable_NEON_FP_traps_EL2_1_0:
    msr CPTR_EL2, xzr
    mov x1, #(0x3 << 20)
    msr CPACR_EL1, x1
    isb
#---------------------------------------------------------------------------------------        
set_new_exception_vector_EL2_1_0:
    ldr   x0,=exception_vector
    msr   VBAR_EL2, x0
    msr   VBAR_EL1, x0
#---------------------------------------------------------------------------------------
prepare_switch_from_EL2_to_EL1:
    mov x0, #0
    orr x0, x0, #(1<<31) // RW
    msr HCR_EL2, x0 
    
    mov x0, #0b0101 
    msr SPSR_EL2, x0
    
    adr x0, el1_entry
    msr ELR_EL2, x0 
    
    msr daifset, #0xF;
#---------------------------------------------------------------------------------------    
switch_to_EL1:  
    eret
#---------------------------------------------------------------------------------------
el1_entry:
    # clear PSTATE.(D,A) and set PSTATE.(I, F)
    # mask every IRQ/FIQ exception
    msr daifclr, #0xC;
    msr daifset, #0x3; 
#---------------------------------------------------------------------------------------
set_new_stack:
	mov	sp, x19    
#---------------------------------------------------------------------------------------
go_to_cpp: 
    mov x0, x20
	ldr x2, =c_entry
    blr  x2
#---------------------------------------------------------------------------------------
should_never_happen:
    bl hang          
#---------------------------------------------------------------------------------------
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Primitives~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#---------------------------------------------------------------------------------------
# infinite-loop
hang: 
    b hang
#---------------------------------------------------------------------------------------
# prints x0 as inverted-hex to uart
hexler:
    mov    x1, #0x21c
    lsl    x1, x1, #16
    mov    x2, #0x600
    add    x3, x1, x2    
    mov    w1, #39
    strb   w1, [x3]    

    hexler_start:
        and     x1, x0, #0xF        
        add     x1, x1, #48
        strb   w1, [x3]        
        lsr    x0, x0, #4        
        cmp     x0, xzr
        b.eq    hexler_end
        b hexler_start
    hexler_end:
        mov    w1, #39
        strb   w1, [x3]  
        mov    w1, #120
        strb   w1, [x3]  
        mov    w1, #48
        strb   w1, [x3]      
        ret  
#---------------------------------------------------------------------------------------
# do nothing
ignore:
    ret
#---------------------------------------------------------------------------------------
# invalidate TLB and data cache
# this code was taken from "u-boot/arch/arm/cpu/armv8/cache.S"
# --> https://github.com/u-boot/u-boot/blob/master/arch/arm/cpu/armv8/cache.S
cache_flush:
    tlbi    alle3
    dsb sy
    isb

    mov  x0, #0
    __asm_flush_dcache_level:
        lsl x12, x0, #1
        msr cssELR_EL1, x12     /* select cache level */
        isb             /* sync change of cssidr_el1 */
        mrs x6, ccsidr_el1      /* read the new cssidr_el1 */
        and x2, x6, #7      /* x2 <- log2(cache line size)-4 */
        add x2, x2, #4      /* x2 <- log2(cache line size) */
        mov x3, #0x3ff
        and x3, x3, x6, lsr #3  /* x3 <- max number of #ways */
        clz w5, w3          /* bit position of #ways */
        mov x4, #0x7fff
        and x4, x4, x6, lsr #13 /* x4 <- max number of #sets */
        /* x12 <- cache level << 1 */
        /* x2 <- line length offset */
        /* x3 <- number of cache ways - 1 */
        /* x4 <- number of cache sets - 1 */
        /* x5 <- bit position of #ways */

    loop_set:
        mov x6, x3          /* x6 <- working copy of #ways */
    loop_way:
        lsl x7, x6, x5
        orr x9, x12, x7     /* map way and level to cisw value */
        lsl x7, x4, x2
        orr x9, x9, x7      /* map set number to cisw value */
        tbz w1, #0, 1f
        dc  isw, x9
        b   2f
        1:  dc  cisw, x9        /* clean & invalidate by set/way */
        2:  subs    x6, x6, #1      /* decrement the way */
        b.ge    loop_way
        subs    x4, x4, #1      /* decrement the set */
        b.ge    loop_set

        ret     

    __asm_dcache_all:
        mov x1, x0
        dsb sy
        mrs x10, clidr_el1      /* read clidr_el1 */
        lsr x11, x10, #24
        and x11, x11, #0x7      /* x11 <- loc */
        cbz x11, finished       /* if loc is 0, exit */
        mov x15, x30
        mov x0, #0          /* start flush at cache level 0 */
        /* x0  <- cache level */
        /* x10 <- clidr_el1 */
        /* x11 <- loc */
        /* x15 <- return address */

    loop_level:
        lsl x12, x0, #1
        add x12, x12, x0        /* x0 <- tripled cache level */
        lsr x12, x10, x12
        and x12, x12, #7        /* x12 <- cache type */
        cmp x12, #2
        b.lt    skip            /* skip if no cache or icache */
        bl  __asm_flush_dcache_level    /* x1 = 0 flush, 1 invalidate */
    skip:
        add x0, x0, #1      /* increment cache level */
        cmp x11, x0
        b.gt    loop_level

        mov x0, #0
        msr cssELR_EL1, x0      /* restore cssELR_EL1 */
        dsb sy
        isb
        mov x30, x15

    finished:
        ret
#---------------------------------------------------------------------------------------
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Exceptions~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#---------------------------------------------------------------------------------------
.macro exception_reg_save
    stp x0, x1, [sp, #-16]!
    stp x2, x3, [sp, #-16]!
    stp x4, x5, [sp, #-16]!
    stp x6, x7, [sp, #-16]!
    stp x8, x9, [sp, #-16]!
    stp x10, x11, [sp, #-16]!
    stp x12, x13, [sp, #-16]!
    stp x14, x15, [sp, #-16]!
    stp x16, x17, [sp, #-16]!
    stp x18, x19, [sp, #-16]!
    
    stp x20, x21, [sp, #-16]!   
    stp x22, x23, [sp, #-16]!   
    stp x24, x25, [sp, #-16]!   
    stp x26, x27, [sp, #-16]!   
    stp x28, x29, [sp, #-16]!  
        
    str x30, [sp, #-16]!
.endm

.macro exception_reg_load    
    ldr x30, [sp], #24
    
    // x19-x28 are callee-save
    // --> no restore necessary
    // --> x19 is restored with x18
    // --> ignore x20-x28

    ldr x29, [sp], #72
    
    ldp x18, x19, [sp], #16
    ldp x16, x17, [sp], #16
    ldp x14, x15, [sp], #16
    ldp x12, x13, [sp], #16
    ldp x10, x11, [sp], #16
    ldp x8, x9, [sp], #16
    ldp x6, x7, [sp], #16
    ldp x4, x5, [sp], #16
    ldp x2, x3, [sp], #16
    ldp x0, x1, [sp], #16
.endm

.align 11
exception_vector:
                //0x000         Synchronous     Current EL with SP0
        exception_reg_save
        mov x0, sp
        ldr x1, exception_sync
        blr x1
        exception_reg_load
        eret

        .align  7 //0x180       IRQ/vIRQ
        exception_reg_save
        mov x0, sp
        ldr x1, exception_IRQ
        blr x1
        exception_reg_load
        eret


        .align  7 //0x100       FIQ/vFIQ
        exception_reg_save
        mov x0, sp
        ldr x1, exception_FIQ
        blr x1
        exception_reg_load
        eret


        .align  7 //0x180       SError/vSError
        exception_reg_save
        mov x0, sp
        ldr x1, exception_SError
        blr x1
        exception_reg_load
        eret


        .align  7 //0x200       Synchronous     Current EL with SPx
        exception_reg_save
        mov x0, sp
        ldr x1, exception_sync
        blr x1
        exception_reg_load
        eret


        .align  7 //0x280       IRQ/vIRQ
        exception_reg_save
        mov x0, sp
        ldr x1, exception_IRQ
        blr x1   
        exception_reg_load
        eret


        .align  7 //0x300       FIQ/vFIQ
        exception_reg_save
        mov x0, sp
        ldr x1, exception_FIQ
        blr x1
        exception_reg_load
        eret


        .align  7 //0x380       SError/vSError
        exception_reg_save
        mov x0, sp
        ldr x1, exception_SError
        blr x1
        exception_reg_load
        eret


        .align  7 //0x400       Synchronous     Lower EL using AArch64
        exception_reg_save
        mov x0, sp
        ldr x1, exception_sync
        blr x1
        exception_reg_load
        eret


        .align  7 //0x480       IRQ/vIRQ
        exception_reg_save
        mov x0, sp
        ldr x1, exception_IRQ
        blr x1
        exception_reg_load
        eret


        .align  7 //0x500       FIQ/vFIQ
        exception_reg_save
        mov x0, sp
        ldr x1, exception_FIQ
        blr x1
        exception_reg_load
        eret


        .align  7 //0x580       SError/vSError
        exception_reg_save
        mov x0, sp
        ldr x1, exception_SError
        blr x1
        exception_reg_load
        eret

        .align  7 //0x600       Synchronous     Lower EL using AArch32
        exception_reg_save
        mov x0, sp
        ldr x1, exception_sync
        blr x1
        exception_reg_load
        eret


        .align  7 //0x680       IRQ/vIRQ
        exception_reg_save
        mov x0, sp
        ldr x1, exception_IRQ
        blr x1
        exception_reg_load
        eret


        .align  7 //0x700       FIQ/vFIQ
        exception_reg_save
        mov x0, sp
        ldr x1, exception_FIQ
        blr x1
        exception_reg_load
        eret
        

        .align  7 //0x780       SError/vSError /
        exception_reg_save
        mov x0, sp
        ldr x1, exception_SError
        blr x1
        exception_reg_load
        eret

       .align 3
       .globl exception_sync
       exception_sync: 

       .align 3
       .globl exception_IRQ
       exception_IRQ: 

       .align 3
       .globl exception_FIQ
       exception_FIQ: 

       .align 3
       .globl exception_SError
       exception_SError: 

