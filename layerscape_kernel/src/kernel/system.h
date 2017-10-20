#ifndef kernel_system_h
#define kernel_system_h

#include <src/stddef.h>
#include "src/util/mutex.h"
#include "src/kernel/memory.h"

namespace kernel {
    // General Module Interface
    // 
    // Each module must implement the following interface:
    // 
    // namespace module {
    //     class example {
    //     public:
    //         struct info {
    //             constexpr static const char* title = "Example";
    //             constexpr static const bool  multi_load = false; // Enable load on secondary-cores
    //         };
    // 
    //         static bool load();
    //     };
    // }
    // 
    // The module can be loaded by calling 'kernel::system::load<module::example>();', which
    // will invoke the load()-methode. The result determines a success-condition.
    //
    // This class provides additional low-level functionality, 
    // like physical-memory allocation (see page_alloc, page_free, page_size), multiple register-methods
    // to set new callback-handlers (see register_exception_IRQ, register_start_application, etc.) and
    // core-identification (see is_core0, set_core_count)
    // 
    // 
    // 
    // 
    class system {
    public:
        static void init();

        // Loads a new module by calling its load-function
        template <typename module>
        static void load() {
            if (is_core0() || module::info::multi_load) {
                if (is_core0()) {
                    log("[Kernel] Load module '", false);
                    log(module::info::title, false);
                    log("'...", false);
                }

                if (module::load()) {
                    if (is_core0()) {
                        log("Success");
                    }
                } else {
                    if (is_core0()) {
                        log("Failure");

                        while (true) {}
                    }
                }
            }

            asm(
                "dsb sy\n"
                "isb"
            );
        }

        static void start_registered_app();

        static byte* page_alloc();
        static bool page_free(byte* addr);
        static int page_size();

        static void register_exception_sync(void (*handler)(byte*));
        static void register_exception_IRQ(void (*handler)(byte*));
        static void register_exception_FIQ(void (*handler)(byte*));
        static void register_exception_SError(void (*handler)(byte*));
        static void register_stdout(void (*stdout)(const char*));
        static void register_start_application(void (*startApp)(void));

        static bool is_core0();
        static void set_core_count(const int count);
        static uint32_t MPIDR();
    private:
        static void log(const char* msg, const bool newline = true);

        constexpr static const int m_early_msg_buffer_size = 1000;

        static char m_early_msg_buffer[m_early_msg_buffer_size + 1];
        static int m_early_msg_buffer_pos;

        static void (*m_stdout)(const char*);
        static void (*m_startApp)(void);

        static int m_pageGranularity;
        static memory m_ddrRegion1;
        static memory m_ddrRegion2;
        static memory m_ddrRegion3;
        static memory m_ddrRegion4;

        static uint32_t m_initCoreID;
        static int m_numCores;
        static std::atomic<int> m_numActivesCores;
        static util::mutex m_mutex;
    };
}

#endif
