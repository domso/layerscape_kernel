#include "src/kernel/memory.h"

namespace kernel {
    bool memory::init(byte* startAddr, byte* endAddr, const uint64_t granularity) {
        m_startAddr = startAddr;
        m_endAddr = endAddr;
        m_currentAddr = m_startAddr;
        m_granularity = granularity;

        m_freeListHead = nullptr;
        m_freeListTail = nullptr;

        return (
                   (((uint64_t)startAddr) & (m_granularity - 1)) == 0 &&
                   (((uint64_t)endAddr) & (m_granularity - 1)) == 0
               );
    }

    byte* memory::page_alloc() {
        byte* result = nullptr;

        if (m_freeListHead == nullptr) {
            if (m_currentAddr < m_endAddr) {
                result = m_currentAddr;
                m_currentAddr += m_granularity;
            }
        } else {
            result = get_free_page();
        }

        return result;
    }

    bool memory::page_free(byte* addr) {
        bool result = false;

        if ((((uint64_t)addr) & (m_granularity - 1)) == 0 && m_startAddr <= addr && addr < m_endAddr) {
            if (addr + m_granularity == m_currentAddr) {
                m_currentAddr -= m_granularity;
            } else {
                insert_free_page(addr);
            }

            result = true;
        }

        return result;
    }

    void memory::insert_free_page(byte* addr) {
        *((byte**)addr) = nullptr;

        if (m_freeListHead == nullptr) {
            m_freeListHead = addr;
        } else {
            *((byte**)m_freeListTail) = addr;
        }

        m_freeListTail = addr;
    }

    byte* memory::get_free_page() {
        byte* result = nullptr;

        if (m_freeListHead != nullptr) {
            result = m_freeListHead;
            m_freeListHead = *((byte**)m_freeListHead);

            if (m_freeListHead == nullptr) {
                m_freeListTail = nullptr;
            }
        }

        return result;
    }
}

