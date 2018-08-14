#include "src/kernel/system.h"

namespace kernel {
void system::init() {
    m_stdout = nullptr;
    m_early_msg_buffer_pos = 0;

    m_startApp = nullptr;
    m_numActivesCores = 0;
    m_numCores = 1;
    m_mutex.init();
}

void system::start_registered_app() {
    if (!is_core0()) {
        m_numActivesCores.fetch_add(1);

        while (m_numActivesCores.load() != m_numCores) {}
    } else {
        while (m_numActivesCores.load() != (m_numCores - 1)) {}

        log("[Kernel] Boot completed on all cores...");
        m_numActivesCores.fetch_add(1);
    }

    if (m_startApp != nullptr) {
        m_startApp();
    }
}

void system::register_stdout(void (*stdout)(const char*)) {
    m_mutex.lock();
    m_stdout = stdout;
    m_early_msg_buffer[m_early_msg_buffer_pos] = '\0';
    log(m_early_msg_buffer);
    m_mutex.unlock();
}

void system::register_start_application(void (*startApp)(void)) {
    m_mutex.lock();
    m_startApp = startApp;
    m_mutex.unlock();
}

bool system::is_core0() {
    return true;
    return m_initCoreID == MPIDR();
}

void system::set_core_count(const int count) {
    m_numCores = count;
//         asm volatile("dsb st");
}

void system::log(const char* msg) {
    if (m_stdout != nullptr) {
        m_stdout(msg);
    } else {
        for (int i = 0; msg[i] != '\0'; i++) {
            m_early_msg_buffer[m_early_msg_buffer_pos] = msg[i];
            m_early_msg_buffer_pos = (m_early_msg_buffer_pos + 1) % m_early_msg_buffer_size;
        }
    }
}

char system::m_early_msg_buffer[m_early_msg_buffer_size + 1];
int system::m_early_msg_buffer_pos;

void (*system::m_stdout)(const char*);
void (*system::m_startApp)();

uint32_t system::m_initCoreID;
int system::m_numCores;
std::atomic<int> system::m_numActivesCores;
util::mutex system::m_mutex;
}
