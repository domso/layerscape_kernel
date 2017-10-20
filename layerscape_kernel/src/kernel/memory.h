#ifndef kernel_memory_h
#define kernel_memory_h

#include <src/stddef.h>

namespace kernel {
    // Controller for an address-range in physical memory
    // Allows the allocation of pages with a given size(=granularity)
    class memory {
    public:
        bool init(byte* startAddr, byte* endAddr, const uint64_t granularity);
        
        byte* page_alloc();        
        bool page_free(byte* addr);
    private:
        void insert_free_page(byte* addr);
        byte* get_free_page();

        byte* m_startAddr;
        byte* m_endAddr;
        byte* m_currentAddr;

        byte* m_freeListHead;
        byte* m_freeListTail;

        uint64_t m_granularity;
    };
}

#endif
