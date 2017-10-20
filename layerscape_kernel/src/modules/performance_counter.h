#ifndef modules_performance_counter_h
#define modules_performance_counter_h

#include <src/stddef.h>
#include <src/kernel/system.h>

namespace module {
    // see ARM Architecture Reference Manual Page 2629
    // Usage:
    // 
    // -Step 0: Check if port is available with port_available()
    // 
    // -Step 1: Check if event is available with event_available()
    // 
    // -Step 2: Set event for port0 by calling port0::listen()
    // 
    // -Step 3: Clear old counter values by calling port0::clear()
    // 
    // -Step 4: Start listening to events by enabling the port0 with enable<port0>
    // 
    // -Step 5: Read current counter value by calling port0::read()
    class performance_counter {        
    public:
        struct info {
            constexpr static const char* title = "Performance Counter";
            constexpr static const bool  multi_load = true;
        };

        static bool load();

        struct events {
            constexpr static const uint32_t SW_INCR                     = 0x0000;       //Instruction architecturally executed, Condition code checkpass, software increment
            constexpr static const uint32_t L1I_CACHE_REFILL            = 0x0001;       //Attributable Level 1 instruction cache refill
            constexpr static const uint32_t L1I_TLB_REFILL              = 0x0002;       //Attributable Level 1 instruction TLB refill
            constexpr static const uint32_t L1D_CACHE_REFILL            = 0x0003;       //Attributable Level 1 data cache refill
            constexpr static const uint32_t L1D_CACHE                   = 0x0004;       //Attributable Level 1 data cache access
            constexpr static const uint32_t L1D_TLB_REFILL              = 0x0005;       //Attributable Level 1 data TLB refill
            constexpr static const uint32_t LD_RETIRED                  = 0x0006;       //Instruction architecturally executed, Condition code check pass, load
            constexpr static const uint32_t ST_RETIRED                  = 0x0007;       //Instruction architecturally executed, Condition code check pass, store
            constexpr static const uint32_t INST_RETIRED                = 0x0008;       //Instruction architecturally executed
            constexpr static const uint32_t EXC_TAKEN                   = 0x0009;       //Exception taken
            constexpr static const uint32_t EXC_RETURN                  = 0x000A;       //Instruction architecturally executed, Condition code check pass, exception return
            constexpr static const uint32_t CID_WRITE_RETIRED           = 0x000B;       //Instruction architecturally executed, Condition code check pass, write to CONTEXTIDR
            constexpr static const uint32_t PC_WRITE_RETIRED            = 0x000C;       //Instruction architecturally executed, Condition code check pass, software change of the PC
            constexpr static const uint32_t BR_IMMED_RETIRED            = 0x000D;       //Instruction architecturally executed, immediate branch
            constexpr static const uint32_t BR_RETURN_RETIRED           = 0x000E;       //Instruction architecturally executed, Condition code check pass, procedure return
            constexpr static const uint32_t UNALIGNED_LDST_RETIRED      = 0x000F;       //Instruction architecturally executed, Condition code check pass, unaligned load or store
            constexpr static const uint32_t BR_MIS_PRED                 = 0x0010;       //Mispredicted or not predicted branch speculatively executed
            constexpr static const uint32_t CPU_CYCLES                  = 0x0011;       //Cycle
            constexpr static const uint32_t BR_PRED                     = 0x0012;       //Predictable branch speculatively executed
            constexpr static const uint32_t MEM_ACCESS                  = 0x0013;       //Data memory access
            constexpr static const uint32_t L1I_CACHE                   = 0x0014;       //Attributable Level 1 instruction cache access
            constexpr static const uint32_t L1D_CACHE_WB                = 0x0015;       //Attributable Level 1 data cache write-back
            constexpr static const uint32_t L2D_CACHE                   = 0x0016;       //Attributable Level 2 data cache access
            constexpr static const uint32_t L2D_CACHE_REFILL            = 0x0017;       //a Attributable Level 2 data cache refill
            constexpr static const uint32_t L2D_CACHE_WB                = 0x0018;       //Attributable Level 2 data cache write-back
            constexpr static const uint32_t BUS_ACCESS                  = 0x0019;       //Bus access
            constexpr static const uint32_t MEMORY_ERROR                = 0x001A;       //Local memory error
            constexpr static const uint32_t INST_SPEC                   = 0x001B;       //Operation speculatively executed
            constexpr static const uint32_t TTBR_WRITE_RETIRED          = 0x001C;       //Instruction architecturally executed, Condition code check pass, write to TTBR
            constexpr static const uint32_t BUS_CYCLES                  = 0x001D;       //Bus cycle
            constexpr static const uint32_t CHAIN                       = 0x001E;       //For odd-numbered counters, increments the count by one for each overflow of the preceding even-numbered counter. For even-numbered counters, there is no increment.
            constexpr static const uint32_t L1D_CACHE_ALLOCATE          = 0x001F;       //Attributable Level 1 data cache allocation without refill
            constexpr static const uint32_t L2D_CACHE_ALLOCATE          = 0x0020;       //Attributable Level 2 data cache allocation without refill
            constexpr static const uint32_t BR_RETIRED                  = 0x0021;       //Instruction architecturally executed, branch
            constexpr static const uint32_t BR_MIS_PRED_RETIRED         = 0x0022;       //Instruction architecturally executed, mispredicted branch
            constexpr static const uint32_t STALL_FRONTEND              = 0x0023;       //No operation issued due to the frontend
            constexpr static const uint32_t STALL_BACKEND               = 0x0024;       //No operation issued due to backend
            constexpr static const uint32_t L1D_TLB                     = 0x0025;       //Attributable Level 1 data or unified TLB access
            constexpr static const uint32_t L1I_TLB                     = 0x0026;       //Attributable Level 1 instruction TLB access
            constexpr static const uint32_t L2I_CACHE                   = 0x0027;       //Attributable Level 2 instruction cache access
            constexpr static const uint32_t L2I_CACHE_REFILL            = 0x0028;       //Attributable Level 2 instruction cache refill
            constexpr static const uint32_t L3D_CACHE_ALLOCATE          = 0x0029;       //Attributable Level 3 data or unified cache allocation without refill
            constexpr static const uint32_t L3D_CACHE_REFILL            = 0x002A;       //Attributable Level 3 data or unified cache refill
            constexpr static const uint32_t L3D_CACHE                   = 0x002B;       //Attributable Level 3 data or unified cache access
            constexpr static const uint32_t L3D_CACHE_WB                = 0x002C;       //Attributable Level 3 data or unified cache write-back
            constexpr static const uint32_t L2D_TLB_REFILL              = 0x002D;       //Attributable Level 2 data or unified TLB refill
            constexpr static const uint32_t L2I_TLB_REFILL              = 0x002E;       //Attributable Level 2 instruction TLB refill
            constexpr static const uint32_t L2D_TLB                     = 0x002F;       //Attributable Level 2 data or unified TLB access
            constexpr static const uint32_t L2I_TLB                     = 0x0030;       //Attributable Level 2 instruction TLB access
        };

        struct port_options {
            constexpr static const uint32_t P                           = 0x80000000;   //Privileged filtering bit
            constexpr static const uint32_t U                           = 0x40000000;   //User filtering bit
            constexpr static const uint32_t NSK                         = 0x20000000;   //Non-secure-EL1 filtering bit
            constexpr static const uint32_t NSU                         = 0x10000000;   //Non-secure-EL0 filtering bit
            constexpr static const uint32_t NSH                         = 0x08000000;   //Non-secure-EL2 filtering bit
            constexpr static const uint32_t M                           = 0x04000000;   //EL3 filtering bit
            constexpr static const uint32_t MT                          = 0x02000000;   //Multithreading (count from all cores!)
            constexpr static const uint32_t DEFAULT                     = 0xAC000000;   //only user-mode and no multithreading
        };

        static bool event_available(uint32_t event);

        template <typename T>
        static void enable() {
            asm(
                "mov    x0, %[mask]\n"
                "msr    PMCNTENSET_EL0, x0"
                :: [mask] "r"(T::mask)
            );
        }

        template <typename T>
        static void disable() {
            asm(
                "mov    x0, %[mask]\n"
                "msr    PMCNTENCLR_EL0, x0"
                :: [mask] "r"(T::mask)
            );
        }

        template <typename T>
        static bool port_available() {
            uint32_t pmcr;

            asm(
                "mrs    %[pmcr], PMCR_EL0\n"
                : [pmcr] "=r"(pmcr)
            );

            pmcr = (pmcr >> 11) & 0x1F;

            return T::mask < pmcr;
        }

        //always available
        struct cycle_counter {
            constexpr static const uint32_t mask = 0x80000000;
            static uint32_t read();
            static void clear();
        };

        struct port0 {
            constexpr static const uint32_t mask = 0x1;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port1 {
            constexpr static const uint32_t mask = 0x2;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port2 {
            constexpr static const uint32_t mask = 0x4;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port3 {
            constexpr static const uint32_t mask = 0x8;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port4 {
            constexpr static const uint32_t mask = 0x10;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port5 {
            constexpr static const uint32_t mask = 0x20;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port6 {
            constexpr static const uint32_t mask = 0x40;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port7 {
            constexpr static const uint32_t mask = 0x80;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port8 {
            constexpr static const uint32_t mask = 0x100;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port9 {
            constexpr static const uint32_t mask = 0x200;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port10 {
            constexpr static const uint32_t mask = 0x400;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port11 {
            constexpr static const uint32_t mask = 0x800;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port12 {
            constexpr static const uint32_t mask = 0x1000;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port13 {
            constexpr static const uint32_t mask = 0x2000;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port14 {
            constexpr static const uint32_t mask = 0x4000;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port15 {
            constexpr static const uint32_t mask = 0x8000;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port16 {
            constexpr static const uint32_t mask = 0x10000;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port17 {
            constexpr static const uint32_t mask = 0x20000;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port18 {
            constexpr static const uint32_t mask = 0x40000;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port19 {
            constexpr static const uint32_t mask = 0x80000;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port20 {
            constexpr static const uint32_t mask = 0x100000;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port21 {
            constexpr static const uint32_t mask = 0x200000;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port22 {
            constexpr static const uint32_t mask = 0x400000;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port23 {
            constexpr static const uint32_t mask = 0x800000;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port24 {
            constexpr static const uint32_t mask = 0x1000000;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port25 {
            constexpr static const uint32_t mask = 0x2000000;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port26 {
            constexpr static const uint32_t mask = 0x4000000;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port27 {
            constexpr static const uint32_t mask = 0x8000000;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port28 {
            constexpr static const uint32_t mask = 0x10000000;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port29 {
            constexpr static const uint32_t mask = 0x20000000;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };

        struct port30 {
            constexpr static const uint32_t mask = 0x40000000;
            static void listen(const uint32_t event, const uint32_t options = port_options::DEFAULT);
            static uint32_t read();
            static void clear();
        };
    };
}

#endif
