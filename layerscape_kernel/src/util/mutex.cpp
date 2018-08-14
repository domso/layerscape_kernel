#include "src/util/mutex.h"

namespace util {
    void mutex::init() {
        m_isLocked = false;
    }

    void mutex::lock() {
        bool current;

//         asm volatile("dsb st");

        do {
            current = false;
        } while (!m_isLocked.compare_exchange_strong(current, true));
    }

    void mutex::unlock() {
//         asm volatile("dsb st");
        m_isLocked = false;
    }

    bool mutex::try_lock() {
        bool current = false;
//         asm volatile("dsb st");
        return m_isLocked.compare_exchange_strong(current, true);
    }
}
