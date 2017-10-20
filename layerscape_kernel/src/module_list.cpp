#include "kernel/system.h"
#include "modules/general_interrupt_controller.h"
#include "modules/exceptions.h"
#include "modules/interrupt_handler.h"
#include "modules/uart.h"
#include "modules/hang.h"
#include "modules/mmu/controller.h"
#include "modules/syscall/controller.h"
#include "modules/syscall/io.h"
#include "modules/syscall/mem.h"
#include "modules/syscall/thread.h"
#include "modules/timer.h"
#include "modules/scheduler/controller.h"
#include "modules/multicore.h"
#include "modules/performance_counter.h"
#include "modules/syscall/performance.h"

// Loads all modules in a sequential order
void module_list() {
    kernel::system::load<module::general_interrupt_controller>();
    kernel::system::load<module::mmu::controller>();
    kernel::system::load<module::exceptions>();
    kernel::system::load<module::interrupt_handler>();
    kernel::system::load<module::uart>();
    kernel::system::load<module::hang>();
    kernel::system::load<module::timer>();
    kernel::system::load<module::syscall::controller>();
    kernel::system::load<module::syscall::io>();
    kernel::system::load<module::syscall::mem>();
    kernel::system::load<module::syscall::thread>();
    kernel::system::load<module::syscall::performance>();
    
    kernel::system::load<module::scheduler::controller>();
    kernel::system::load<module::multicore>();
    kernel::system::load<module::performance_counter>();

    if (kernel::system::is_core0()) {
        // create first application-process
        byte* code = (byte*) 0xb0000000;
        module::scheduler::controller::create_process(code, 4096, code);
    }   
}
