#ifndef modules_scheduler_process_list_h
#define modules_scheduler_process_list_h

#include "src/modules/scheduler/process.h"

namespace module {
    namespace scheduler {
        class process_list {
        public:
            void init();
            module::scheduler::process* get_next();
            void push(module::scheduler::process* process);
            void remove_and_update(module::scheduler::process* process);
            bool wait_for(const uint64_t id, module::scheduler::process* process);

            void print();

            module::scheduler::process* search_for_id(const uint64_t id);
        private:
            module::scheduler::process* internal_pop();
            void internal_push(module::scheduler::process* process);

            module::scheduler::process* head;
            module::scheduler::process* tail;
            util::mutex mutex;
        };
    }
}

#endif
