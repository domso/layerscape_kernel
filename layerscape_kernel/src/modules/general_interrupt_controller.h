#ifndef module_general_interrupt_controller_h
#define module_general_interrupt_controller_h

#include <src/stddef.h>
#include "src/kernel/system.h"

namespace module {
    // ARM Generic Interrupt Controller Architecture Specification
    // GIC module version 3.0 and version 4.0
    // U-Boot does a lot of initializiation!
    // --> https://github.com/u-boot/u-boot/blob/master/arch/arm/lib/gic_64.S
    class general_interrupt_controller {
    public:
        struct info {
            constexpr static const char* title = "ARM-GICv3/4 Wrapper";
            constexpr static const bool  multi_load = true;
        };

        static bool load();

        struct interface {
            struct distributor_map {
                uint32_t GICD_CTLR;                 //See the register description Distributor Control Register
                const uint32_t GICD_TYPER;          //Interrupt Controller Type Register
                const uint32_t GICD_IIDR;           //Distributor Implementer Identification Register
                uint32_t reserved;                  //Reserved
                uint32_t GICD_STATUSR;              //Error Reporting Status Register, optional
                uint32_t reserved2[3];              //Reserved
                uint32_t reserved3[8];
                uint32_t GICD_SETSPI_NSR;           //Set SPI Register
                uint32_t reserved4;                 //Reserved
                uint32_t GICD_CLRSPI_NSR;           //Clear SPI Register
                uint32_t reserved5;                 //Reserved
                uint32_t GICD_SETSPI_SR;            //Set SPI, Secure Register
                uint32_t reserved6;                 //Reserved
                uint32_t GICD_CLRSPI_SR;            //Clear SPI, Secure Register
                uint32_t  reserved7[9] ;            //Reserved
                uint32_t GICD_IGROUPR[32];          //Interrupt Group Registers

                uint32_t GICD_ISENABLER[32];        //Interrupt Set-Enable Registers
                uint32_t GICD_ICENABLER[32];        //Interrupt Clear-Enable Registers
                uint32_t GICD_ISPENDR[32];          //Interrupt Set-Pending Registers
                uint32_t GICD_ICPENDR[32];          //Interrupt Clear-Pending Registers
                uint32_t GICD_ISACTIVER[32];        //Interrupt Set-Active Registers
                uint32_t GICD_ICACTIVER[32];        //Interrupt Clear-Active Registers
                uint32_t GICD_IPRIORITYR[256];      //Interrupt Priority Registers
                const uint32_t GICD_ITARGETSR[256]; //Interrupt Processor Targets Registers

                uint32_t  GICD_ICFGR[64];           //Interrupt Configuration Registers
                uint32_t  GICD_IGRPMODR[64];        //Interrupt Group Modifier Registers

                uint32_t  GICD_NSACR[64];           //Non-secure Access Control Registers
                uint32_t  GICD_SGIR[4];             //Software Generated Interrupt Register
                uint32_t  GICD_CPENDSGIR[4];        //SGI Clear-Pending Registers
                uint32_t  GICD_SPENDSGIR[4];        //SGI Set-Pending Registers
                uint32_t  reserved8[4];             //Reserved
                uint32_t  GICD_IROUTER[1976];       //Interrupt Routing Registers
                uint32_t  reserved9[4104];          //Reserved
                uint32_t  reserved10[4084];         //IMPLEMENTATION DEFINED
                const uint32_t reserved11[12];      //Reserved for ID registers
            };

            // dont use
            struct cpu_map {
                uint32_t GICC_CTLR;                 //CPU Interface Control Register
                uint32_t GICC_PMR;                  //Interrupt Priority Mask Register
                uint32_t GICC_BPR;                  //Binary Point Register
                uint32_t GICC_IAR;                  //Interrupt Acknowledge Register
                uint32_t GICC_EOIR;                 //End of Interrupt Register
                uint32_t GICC_RPR;                  //Running Priority Register
                uint32_t GICC_HPPIR;                //Highest Priority Pending Interrupt Register
                uint32_t GICC_ABPR;                 //Aliased Binary Point Register
                uint32_t GICC_AIAR;                 //Aliased Interrupt Acknowledge Register
                uint32_t GICC_AEOIR;                //Aliased End of Interrupt Register
                uint32_t GICC_AHPPIR;               //Aliased Highest Priority Pending Interrupt Register
                uint32_t GICC_STATUSR;              //Error Reporting Status Register
                uint32_t reserved1[4];
                uint32_t reserved2[36];
                uint32_t GICC_APRn[4];              //Active Priorities Registers
                uint8_t GICC_NSAPRn[13];            //Non-secure Active Priorities Registers
                uint8_t reserved3[15];
                uint32_t GICC_IIDR;                 //CPU Interface Identification Register
                uint32_t reserved5[961];
                uint32_t GICC_DIR;                  //Deactivate Interrupt Register
            };

            struct RD_base_map {
                uint32_t GICR_CTLR;                 //See the register description Redistributor Control Register
                const uint32_t GICR_IIDR;           //Implementer Identification Register
                const uint64_t GICR_TYPER;          //Redistributor Type Register
                uint32_t GICR_STATUSR;              //Error Reporting Status Register, optional
                uint32_t GICR_WAKER;                //See the register description Redistributor Wake Register
                uint64_t reserved1;                 //Reserved
                uint32_t reserved2[8];                 //IMPLEMENTATION DEFINED
                uint64_t GICR_SETLPIR;              //Set LPI Pending Register
                uint64_t GICR_CLRLPIR;              //Clear LPI Pending Register
                uint32_t reserved3[8] ;             //Reserved
                uint64_t GICR_PROPBASER;            //Redistributor Properties Base Address Register
                uint64_t GICR_PENDBASER;            //Redistributor LPI Pending Table Base Address Register
                uint32_t reserved4[8] ;             //Reserved
                uint64_t GICR_INVLPIR;              //Redistributor Invalidate LPI Register

                uint64_t reserved5;                 //Reserved
                uint64_t GICR_INVALLR;              //Redistributor Invalidate All Register
                uint64_t reserved6;                 //Reserved
                const uint64_t GICR_SYNCR;          //Redistributor Synchronize Register
                uint64_t reserved7[7];                 //Reserved
                uint64_t reserved8;                 //IMPLEMENTATION DEFINED
                uint64_t reserved9;                 //Reserved
                uint64_t reserved10;                //IMPLEMENTATION DEFINED
                uint64_t reserved11[6109];          //Reserved
                uint32_t reserved12[4084];           //IMPLEMENTATION DEFINED
                uint32_t reserved13[12];             //Reserved for ID registers, see Identification registers on page 8-170
            };

            struct SGI_base_map {
                uint32_t reserved0[32];
                uint32_t GICR_IGROUPR[32];          //Interrupt Group Register 0
                uint32_t GICR_ISENABLER[32];        //Interrupt Set-Enable Register 0
                uint32_t GICR_ICENABLER[32];        //Interrupt Clear-Enable Register 0
                uint32_t GICR_ISPENDR[32];          //Interrupt Set-Pend Register 0
                uint32_t GICR_ICPENDR[32];          //Interrupt Clear-Pend Register 0
                uint32_t GICR_ISACTIVER[32];        //Interrupt Set-Active Register 0
                uint32_t GICR_ICACTIVER[32];        //Interrupt Clear-Active Register 0
                uint32_t GICR_IPRIORITYR[512];      //Interrupt Priority Registers
                uint32_t GICR_ICFGR0;               //SGI Configuration Register
                uint32_t GICR_ICFGR1;               //PPI Configuration Register
                uint32_t reserved1[62];
                uint32_t GICR_IGRPMODR0[64];        //Interrupt Group Modifier Register 0
                uint32_t GICR_NSACR;                //Non-Secure Access Control Register
                uint32_t reserved2[11391];          //Reserved
                uint32_t reserved3[4084];           //IMPLEMENTATION DEFINED
                uint32_t reserved4[12];             //Reserved
            };

            struct VLPI_base_map {
                uint32_t reserved0[16];             //Reserved
                uint32_t reserved1[4];              //IMPLEMENTATION DEFINED IMPLEMENTATION DEFINED
                uint32_t reserved2[8];              //Reserved
                uint64_t GICR_VPROPBASER;           //Virtual Redistributor Properties Base Address Register
                uint64_t GICR_VPENDBASER;           //Virtual Pending Table Base Address Register
                uint32_t reserved3[192];            //IMPLEMENTATION DEFINED
                uint32_t reserved4[12064];          //Reserved
                uint32_t reserved5[4084];           //IMPLEMENTATION DEFINED
                uint32_t reserved6[12];             //Reserved
            };

            struct redistributor_map {
                RD_base_map rd_base;
                SGI_base_map sgi_base;
            };

            struct cpu {
                struct ICC_IAR0_EL1 {
                    static inline uint32_t load() {
                        asm("mrs     x0, S3_0_C12_C8_0");
                    }

                    static inline void store(const uint32_t) {
                        asm("msr     S3_0_C12_C8_0, x0");
                    }
                };
                struct ICC_IAR1_EL1 {
                    static inline uint32_t load() {
                        asm("mrs     x0, S3_0_C12_C12_0");
                    }

                    static inline void store(const uint32_t) {
                        asm("msr     S3_0_C12_C12_0, x0");
                    }
                };
                struct ICC_EOIR0_EL1 {
                    static inline uint32_t load() {
                        asm("mrs     x0, S3_0_C12_C8_1");
                    }

                    static inline void store(const uint32_t) {
                        asm("msr     S3_0_C12_C8_1, x0");
                    }
                };
                struct ICC_EOIR1_EL1 {
                    static inline uint32_t load() {
                        asm("mrs     x0, S3_0_C12_C12_1");
                    }

                    static inline void store(const uint32_t) {
                        asm("msr     S3_0_C12_C12_1, x0");
                    }
                };

                struct ICC_HPPIR0_EL1 {
                    static inline uint32_t load() {
                        asm("mrs     x0, S3_0_C12_C8_2");
                    }

                    static inline void store(const uint32_t) {
                        asm("msr     S3_0_C12_C8_2, x0");
                    }
                };

                struct ICC_HPPIR1_EL1 {
                    static inline uint32_t load() {
                        asm("mrs     x0, S3_0_C12_C12_2");
                    }

                    static inline void store(const uint32_t) {
                        asm("msr     S3_0_C12_C12_2, x0");
                    }
                };

                struct ICC_BPR0_EL1 {
                    static inline uint32_t load() {
                        asm("mrs     x0, S3_0_C12_C8_3");
                    }

                    static inline void store(const uint32_t) {
                        asm("msr     S3_0_C12_C8_3, x0");
                    }
                };

                struct ICC_BPR1_EL1 {
                    static inline uint32_t load() {
                        asm("mrs     x0, S3_0_C12_C12_3");
                    }

                    static inline void store(const uint32_t) {
                        asm("msr     S3_0_C12_C12_3, x0");
                    }
                };

                struct ICC_DIR_EL1 {
                    static inline uint32_t load() {
                        asm("mrs     x0, S3_0_C12_C11_1");
                    }

                    static inline void store(const uint32_t) {
                        asm("msr     S3_0_C12_C11_1, x0");
                    }
                };

                struct ICC_PMR_EL1 {
                    static inline uint32_t load() {
                        asm("mrs     x0, S3_0_C4_C6_0");
                    }

                    static inline void store(const uint32_t) {
                        asm("msr     S3_0_C4_C6_0, x0");
                    }
                };

                struct ICC_RPR_EL1 {
                    static inline uint32_t load() {
                        asm("mrs     x0, S3_0_C12_C11_3");
                    }

                    static inline void store(const uint32_t) {
                        asm("msr     S3_0_C12_C11_3, x0");
                    }
                };

                struct ICC_CTLR_EL1 {
                    static inline uint32_t load() {
                        asm("mrs     x0, S3_0_C12_C12_4");
                    }

                    static inline void store(const uint32_t) {
                        asm("msr     S3_0_C12_C12_4, x0");
                    }
                };

                struct ICC_CTLR_EL3 {
                    static inline uint32_t load() {
                        asm("mrs     x0, S3_6_C12_C12_4");
                    }

                    static inline void store(const uint32_t) {
                        asm("msr     S3_6_C12_C12_4, x0");
                    }
                };

                struct ICC_SRE_EL1 {
                    static inline uint32_t load() {
                        asm("mrs     x0, S3_0_C12_C12_5");
                    }

                    static inline void store(const uint32_t) {
                        asm("msr     S3_0_C12_C12_5, x0");
                    }
                };

                struct S3_0_C12_C12_5 {
                    static inline uint32_t load() {
                        asm("mrs     x0, S3_4_C12_C9_5");
                    }

                    static inline void store(const uint32_t) {
                        asm("msr     S3_4_C12_C9_5, x0");
                    }
                };

                struct ICC_SRE_EL3 {
                    static inline uint32_t load() {
                        asm("mrs     x0, S3_6_C12_C12_5");
                    }

                    static inline void store(const uint32_t) {
                        asm("msr     S3_6_C12_C12_5, x0");
                    }
                };

                struct ICC_IGRPEN0_EL1 {
                    static inline uint32_t load() {
                        asm("mrs     x0, S3_0_C12_C12_6");
                    }

                    static inline void store(const uint32_t) {
                        asm("msr     S3_0_C12_C12_6, x0");
                    }
                };

                struct ICC_IGRPEN1_EL1 {
                    static inline uint32_t load() {
                        asm("mrs     x0, S3_0_C12_C12_7");
                    }

                    static inline void store(const uint32_t) {
                        asm("msr     S3_0_C12_C12_7, x0");
                    }
                };

                struct ICC_IGRPEN1_EL3 {
                    static inline uint32_t load() {
                        asm("mrs     x0, S3_6_C12_C12_7");
                    }

                    static inline void store(const uint32_t) {
                        asm("msr     S3_6_C12_C12_7, x0");
                    }
                };

                struct ICC_SEIEN_EL1 {
                    static inline uint32_t load() {
                        asm("mrs     x0, S3_0_C12_C13_0");
                    }

                    static inline void store(const uint32_t) {
                        asm("msr     S3_0_C12_C13_0, x0");
                    }
                };

                struct ICC_SGI0R_EL1 {
                    static inline uint64_t load() {
                        asm("mrs     x0, S3_0_C12_C11_7");
                    }

                    static inline void store(const uint64_t) {
                        asm("msr     S3_0_C12_C11_7, x0");
                    }
                };

                struct ICC_SGI1R_EL1 {
                    static inline uint64_t load() {
                        asm("mrs     x0, S3_0_C12_C11_5");
                    }

                    static inline void store(const uint64_t) {
                        asm("msr     S3_0_C12_C11_5, x0");
                    }
                };

                struct ICC_ASGI1R_EL1 {
                    static inline uint64_t load() {
                        asm("mrs     x0, S3_0_C12_C11_6");
                    }

                    static inline void store(const uint64_t) {
                        asm("msr     S3_0_C12_C11_6, x0");
                    }
                };
            };

            // each cpu has its own redistributor!
            static volatile redistributor_map* redistributor();

            constexpr static volatile distributor_map* distributor = (volatile distributor_map*) 0x06000000;
            constexpr static volatile redistributor_map* redistributor_base = (volatile redistributor_map*) 0x06100000;
            constexpr static volatile redistributor_map* redistributor_base_end = (volatile redistributor_map*) 0x06200000;



            // When affinity routing is enabled for a Security state, it is a programming error to use
            // memory-mapped registers to access the GIC.
            // ARM Generic Interrupt Controller Architecture Specification GIC module version 3.0 and version 4.0 Page: 579
            // constexpr volatile cpu_map* cpu = (volatile cpu_map*) 0x0c0c0000;

        };
    };
}

#endif
