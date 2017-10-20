#include "src/modules/scheduler/process_status.h"

namespace module {
    namespace scheduler {
        void process_status::operator=(const process_status& old) {
            m_currentState.store(old.m_currentState.load());
        }

        bool process_status::is(const uint64_t option) const {
            return (m_currentState.load() & option) != 0;
        }

        void process_status::set(const uint64_t option) {
            uint64_t current;

            do {
                current = m_currentState.load();
            } while (!m_currentState.compare_exchange_weak(current, current | option));
        }

        void process_status::unset(const uint64_t option) {
            uint64_t current;

            do {
                current = m_currentState.load();
            } while (!m_currentState.compare_exchange_weak(current, current & (~option)));
        }

        bool process_status::try_set(const uint64_t option) {
            uint64_t current;
            current = m_currentState.load();
            current = current & (~option);
            return m_currentState.compare_exchange_strong(current, current | option);
        }

        uint64_t process_status::get() {
            return m_currentState.load();
        }

        void process_status::clear() {
            m_currentState.store(0);
        }
    }
}
