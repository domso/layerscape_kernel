#include <stdint.h>
#include "syscalls.h"

void hello(void*) {
    write("Hello User\r\n", 12);
    EXIT(0);
}

void main() {    
    thread_t threads[10];
    
    uint64_t t1 = get_instr_retired();
    
    for (int i = 0; i < 10; i++) {
        threads[i] = thread_create(nullptr, hello, nullptr);
    }
    
    for (int i = 0; i < 10; i++) {
        thread_join(threads[i]);
    
    }
    
    
    uint64_t t2 = get_instr_retired();
    
    
    
    
    EXIT(0);
}