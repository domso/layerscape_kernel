#include "kernel/system.h"
#include "modules/display.h"
#include "modules/keyboard.h"




// Loads all modules in a sequential order
void module_list() {
    kernel::system::load<module::display>();
    kernel::system::load<module::keyboard>();
}
