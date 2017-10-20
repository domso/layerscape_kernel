#ifndef modules_scheduler_process_h
#define modules_scheduler_process_h

#include <src/stddef.h>
#include "src/kernel/system.h"
#include "src/kernel/register_maps.h"
#include "src/modules/scheduler/va_map.h"
#include "src/modules/scheduler/process_status.h"

namespace module {
    namespace scheduler {
        class process {
        public:
            struct attributes {
                uint64_t id;
                uint64_t asid;
                bool static_schedule;
                uint64_t stackSize;
            };

            void init(byte* codePA, const uint64_t codeLength, byte* entryAddress, const module::scheduler::process::attributes& attr);

            process* clone();

            void resume(uint64_t oldStack);
            void store_context(kernel::register_maps::general_registers& saved_registers);

            bool is_static_scheduled() const;

            attributes& config();

            byte* malloc(const uint64_t size, const uint64_t attrIndx);
            void free(const byte* start);

            void setPC(byte* addr);
            
            void signal(const uint64_t signal);           
            
            bool execute_signals();

            bool no_context_store;
            process* parent;
            process* waitfor;
            process* next;  // for process_list
            process_status state;
        private:
            void destroy();
            void init_code(byte* codePA, const uint64_t codeLength, byte* entryAddress);
            void init_stack();

            attributes m_currentConfig;
            kernel::register_maps::general_registers m_saved_general_registers;
            kernel::register_maps::fp_registers m_saved_fp_registers;
            kernel::register_maps::system_registers m_saved_system_registers;
            va_map m_map;
            util::mutex m_mutex;
            util::mutex* m_selectedMutex;
        };
    }
}

#endif
