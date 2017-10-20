#ifndef modules_scheduler_process_status_h
#define modules_scheduler_process_status_h

#include <atomic>

namespace module {
    namespace scheduler {
        class process_status {
        public:
            struct states {
                constexpr static uint64_t running = 0x1;
                constexpr static uint64_t blocked = 0x2;
            };

            struct signals {
                constexpr static uint64_t SIGANY  = 0xFFFFFFFF00000000;
                constexpr static uint64_t SIGINT  = 0x0000000100000000;
                constexpr static uint64_t SIGKILL = 0x0000000200000000;
                constexpr static uint64_t SIGTERM = 0x0000000400000000;
                constexpr static uint64_t SIGSTOP = 0x0000000800000000;
                constexpr static uint64_t SIGCONT = 0x0000001000000000;
            };

            void operator=(const process_status& old);

            bool is(const uint64_t option) const;
            void set(const uint64_t option);
            void unset(const uint64_t option);

            bool try_set(const uint64_t option);

            uint64_t get();
            void clear();
        private:
            std::atomic<uint64_t> m_currentState;
        };
    }
}

#endif
