#include "src/modules/syscall/performance.h"
#include "src/modules/performance_counter.h"
#include "src/modules/syscall/controller.h"

namespace module {
    
    bool syscall::performance::load() {     
	module::syscall::controller::register_syscall<instr_retired>();
      
        module::performance_counter::port0::listen(module::performance_counter::events::INST_RETIRED);
        
        module::performance_counter::enable<module::performance_counter::port0>();
        
        return true;
    }

    void syscall::performance::instr_retired::handle(kernel::register_maps::general_registers& saved_register) {
        saved_register.x0 = module::performance_counter::port0::read();
    }   
}