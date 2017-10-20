#ifndef modules_scheduler_controller_h
#define modules_scheduler_controller_h

#include <src/stddef.h>
#include <atomic>
#include "src/kernel/system.h"
#include "src/util/mutex.h"
#include "src/modules/mmu/table_entry.h"
#include "src/modules/exceptions.h"
#include "src/modules/scheduler/va_map.h"
#include "src/modules/timer.h"
#include "src/modules/scheduler/process_list.h"

namespace module {
    namespace scheduler {
        // Starts application-images directly from memory
        // 
        // An application-image needs to be addressed
        // by its memory-address, entry-point-address and size
        class controller {
        public:
            struct info {
                constexpr static const char* title = "Process Scheduler";
                constexpr static const bool  multi_load = false;
            };

            constexpr static const double tick_per_sec = 0.1;

            static bool load();

            static scheduler::process* create_process(byte* codePA, const int codeLength, byte* entryAddress);

            static uint64_t clone_process(module::scheduler::process& proc, byte* startAddress, module::scheduler::process::attributes* attributes);
            static void context_switch(kernel::register_maps::general_registers& saved_register);
            static void wait_for(const uint64_t id, module::scheduler::process& child);

            static scheduler::process* get_process(const uint64_t id);
            static scheduler::process* get_current_process();
        private:
            struct interrupt_process {
                constexpr static const int id = module::timer::interruptID;
                static void handle(kernel::register_maps::general_registers& saved_registers);
            };

            static void main();

            static scheduler::process* init_process(byte* codePA, const int codeLength, byte* entryAddress);

            static void start_next_process(uint64_t oldStack);

            static void idle();
            static void set_current_process(scheduler::process* newProcess);

            static process_list m_activeProcesses;
            static scheduler::process* m_idleProcess;
            static std::atomic<uint64_t> m_nextProcessID;
        };
    }
}

#endif
