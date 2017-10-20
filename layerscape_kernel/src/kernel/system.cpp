#include "src/kernel/system.h"

namespace kernel {
    void system::init() {
        m_stdout = nullptr;
        m_early_msg_buffer_pos = 0;

        m_pageGranularity = 4096; // 4KB Page-granularity

        byte* kernelEndAddr;
        asm(
            "ldr %[addr], =kernel_memory_end"
            : [addr] "=r"(kernelEndAddr)
        );

        // see bdinfo in u-boot
        // DRAM bank   = 0x00000000
        // -> start    = 0x80000000
        // -> size     = 0x80000000
        // DRAM bank   = 0x00000001
        // -> start    = 0x8080000000
        // -> size     = 0x0340000000
        // DRAM bank   = 0x00000002
        // -> start    = 0x6000000000
        // -> size     = 0x0100000000

        //[0xb0000000, 0xc0000000) is reserved for application binaries
        
        m_ddrRegion1.init(kernelEndAddr, (byte*)0xb0000000, m_pageGranularity);
        m_ddrRegion2.init((byte*)0xc0000000, (byte*)0x100000000, m_pageGranularity);
        m_ddrRegion3.init((byte*)0x8080000000, (byte*)0x83C0000000, m_pageGranularity);
        m_ddrRegion4.init((byte*)0x6000000000, (byte*)0x6100000000, m_pageGranularity);

        m_startApp = nullptr;
        m_numActivesCores = 0;
        m_numCores = 1;

        m_initCoreID = MPIDR();

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

    byte* system::page_alloc() {
        byte* result = nullptr;

        m_mutex.lock();
        result = m_ddrRegion1.page_alloc();

        if (result == nullptr) {
            result = m_ddrRegion2.page_alloc();
        }

        if (result == nullptr) {
            result = m_ddrRegion3.page_alloc();
        }

        if (result == nullptr) {
            result = m_ddrRegion4.page_alloc();
        }

        m_mutex.unlock();

        return result;
    }

    bool system::page_free(byte* addr) {
        bool result;
        m_mutex.lock();
        result = m_ddrRegion1.page_free(addr) || m_ddrRegion2.page_free(addr) || m_ddrRegion3.page_free(addr) || m_ddrRegion4.page_free(addr);
        m_mutex.unlock();
        return result;
    }

    int system::page_size() {
        return m_pageGranularity;
    }

    void system::register_exception_sync(void (*handler)(byte*)) {
        m_mutex.lock();
        volatile void (**current)(byte*);
        asm(
            "ldr %[handler], =exception_sync"
            : [handler] "=r"(current)
        );
        *current = (volatile void(*)(byte*))handler;
        m_mutex.unlock();
    }

    void system::register_exception_IRQ(void (*handler)(byte*)) {
        m_mutex.lock();
        volatile void (**current)(byte*);
        asm(
            "ldr %[handler], =exception_IRQ"
            : [handler] "=r"(current)
        );
        *current = (volatile void(*)(byte*))handler;
        m_mutex.unlock();
    }

    void system::register_exception_FIQ(void (*handler)(byte*)) {
        m_mutex.lock();
        volatile void (**current)(byte*);
        asm(
            "ldr %[handler], =exception_FIQ"
            : [handler] "=r"(current)
        );
        *current = (volatile void(*)(byte*))handler;
        m_mutex.unlock();
    }

    void system::register_exception_SError(void (*handler)(byte*)) {
        m_mutex.lock();
        volatile void (**current)(byte*);
        asm(
            "ldr %[handler], =exception_SError"
            : [handler] "=r"(current)
        );
        *current = (volatile void(*)(byte*))handler;
        m_mutex.unlock();
    }

    void system::register_stdout(void (*stdout)(const char*)) {
        m_mutex.lock();
        m_stdout = stdout;
        m_early_msg_buffer[m_early_msg_buffer_pos] = '\0';
        log(m_early_msg_buffer, false);
        m_mutex.unlock();
    }

    void system::register_start_application(void (*startApp)(void)) {
        m_mutex.lock();
        m_startApp = startApp;
        m_mutex.unlock();
    }

    bool system::is_core0() {
        return m_initCoreID == MPIDR();
    }

    void system::set_core_count(const int count) {
        m_numCores = count;
        asm volatile("dsb st");
    }

    uint32_t system::MPIDR() {
        asm(
            "mrs    x0,  mpidr_el1\n"
            "mov    x1, x0\n"
            "and    x0, x0, #0xFFFFFF\n"
            "lsr    x1, x1, #8\n"
            "and    x1, x1, #0xFF000000\n"
            "add    x0, x0, x1\n"
        );
    }

    void system::log(const char* msg, const bool newline) {
        if (m_stdout != nullptr) {
            m_stdout(msg);

            if (newline) {
                m_stdout("\r\n");
            }
        } else {
            for (int i = 0; msg[i] != '\0'; i++) {
                m_early_msg_buffer[m_early_msg_buffer_pos] = msg[i];
                m_early_msg_buffer_pos = (m_early_msg_buffer_pos + 1) % m_early_msg_buffer_size;
            }

            if (newline) {
                m_early_msg_buffer[m_early_msg_buffer_pos] = '\r';
                m_early_msg_buffer_pos = (m_early_msg_buffer_pos + 1) % m_early_msg_buffer_size;
                m_early_msg_buffer[m_early_msg_buffer_pos] = '\n';
                m_early_msg_buffer_pos = (m_early_msg_buffer_pos + 1) % m_early_msg_buffer_size;
            }
        }
    }

    char system::m_early_msg_buffer[m_early_msg_buffer_size + 1];
    int system::m_early_msg_buffer_pos;

    void (*system::m_stdout)(const char*);
    void (*system::m_startApp)();

    int system::m_pageGranularity;
    memory system::m_ddrRegion1;
    memory system::m_ddrRegion2;
    memory system::m_ddrRegion3;
    memory system::m_ddrRegion4;

    uint32_t system::m_initCoreID;
    int system::m_numCores;
    std::atomic<int> system::m_numActivesCores;
    util::mutex system::m_mutex;
}
