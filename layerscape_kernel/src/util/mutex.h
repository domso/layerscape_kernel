#ifndef util_mutex_h
#define util_mutex_h

#include <atomic>

namespace util {
    // base-mutex
    // DOES NOT WORK CORRECTLY BEFORE MMU WAS INITIALIZED! (i think so)
    class mutex {
    public:
        void init();
        void lock();
        void unlock();
        bool try_lock();
    private:
        std::atomic<bool> m_isLocked;
    };
}

#endif
