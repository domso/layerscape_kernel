#include "src/modules/timer.h"
#include "src/modules/uart.h"
#include "src/modules/interrupt_handler.h"
#include "src/modules/general_interrupt_controller.h"

namespace module {
    bool timer::load() {
        if (kernel::system::is_core0()) {
            module::interrupt_handler::register_interrupt_handler<default_timer_handler>();
        }

        disable_timer();

        module::general_interrupt_controller::interface::redistributor()->sgi_base.GICR_ISENABLER[0] = 0xFFFFFFFF;
    }

    void timer::set_freq(const int32_t hz) {
        asm(
            "msr    CNTFRQ_EL0, x0\n"
        );
    }

    int32_t timer::get_freq() {
        asm(
            "mrs    x0, CNTFRQ_EL0\n"
        );
    }

    void timer::enable_timer() {
        asm(
            "mov    x0, #1\n"
            "msr    CNTP_CTL_EL0, x0\n"
        );
    }

    void timer::disable_timer() {
        asm(
            "msr    CNTP_CTL_EL0, xzr\n"
        );
    }

    void timer::set_timer(const int32_t time) {
        asm(
            "mrs    x1, CNTPCT_EL0\n"
            "add    x0, x0, x1\n"
            "msr    CNTP_CVAL_EL0, x0\n"
        );
    }

    void timer::set_abs_timer(const int32_t time) {
        asm(
            "msr    CNTP_CVAL_EL0, x0\n"
        );
    }

    void timer::set_timer_sec(const double time) {
        set_timer(time * get_freq());
    }

    void timer::default_timer_handler::handle(kernel::register_maps::general_registers& saved_registers) {
        module::uart::println("Received timer-interrupt");
        disable_timer();
        set_timer_sec(5);
    }
}



