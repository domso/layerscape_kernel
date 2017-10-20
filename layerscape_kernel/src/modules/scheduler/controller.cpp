#include "src/modules/scheduler/controller.h"
#include "src/modules/uart.h"
#include "src/modules/mmu/controller.h"
#include "src/modules/scheduler/el0_start.h"
#include "src/modules/interrupt_handler.h"
#include <src/kernel/register_maps.h>
#include "src/modules/general_interrupt_controller.h"

#include "src/modules/performance_counter.h"

namespace module {
    namespace scheduler {
        bool controller::load() {
            m_activeProcesses.init();
            module::interrupt_handler::register_interrupt_handler<interrupt_process>();
            kernel::system::register_start_application(main);
            m_nextProcessID = 1;

            m_idleProcess = init_process((byte*)idle, 4096, (byte*)idle);
            m_idleProcess->no_context_store = true;

            set_current_process(nullptr);

            return true;
        }

        void controller::main() {
            while (true) {
                start_next_process(0);
            }
        }

        scheduler::process* controller::create_process(byte* codePA, const int codeLength, byte* entryAddress) {
            scheduler::process* result = init_process(codePA, codeLength, entryAddress);

            if (result != nullptr) {
                m_activeProcesses.push(result);
            }

            return result;
        }

        scheduler::process* controller::init_process(byte* codePA, const int codeLength, byte* entryAddress) {
            scheduler::process* result = nullptr;

            if (sizeof(scheduler::process) < kernel::system::page_size()) {
                scheduler::process* newProcess = (scheduler::process*)kernel::system::page_alloc();

                if (newProcess != nullptr) {
                    scheduler::process::attributes processConfig;
                    processConfig.id = m_nextProcessID.fetch_add(1);
                    processConfig.asid = processConfig.id;
                    processConfig.static_schedule = true;
                    processConfig.stackSize = 4096;

                    newProcess->init(codePA, codeLength, entryAddress, processConfig);

                    result = newProcess;
                }
            }

            return result;
        }

        uint64_t controller::clone_process(module::scheduler::process& proc, byte* startAddress, module::scheduler::process::attributes* attributes) {
            uint64_t result = 0;
            module::scheduler::process* newProcess = proc.clone();

            if (newProcess != nullptr) {
                newProcess->config().id = m_nextProcessID.fetch_add(1);

                if (attributes != nullptr) {
                    attributes->id = newProcess->config().id;
                    attributes->asid = newProcess->config().asid;

                    newProcess->config() = *attributes;
                }

                if (startAddress != nullptr) {
                    newProcess->setPC(startAddress);
                }

                result = newProcess->config().id;
                m_activeProcesses.push(newProcess);
            }

            return result;
        }

        void controller::context_switch(kernel::register_maps::general_registers& saved_register) {
            scheduler::process* currentProcess = get_current_process();

            if (currentProcess != nullptr) {
                currentProcess->store_context(saved_register);
                currentProcess->state.unset(scheduler::process_status::states::running);
            }

            //&saved_register is mapped to the stackpointer after the 'exception_reg_save'-call in startup.S
            uint64_t oldStack = (uint64_t)&saved_register;
            oldStack += sizeof(saved_register);

            while (true) {
                start_next_process(oldStack);
            }
        }

        void controller::wait_for(const uint64_t id, scheduler::process& child) {
            m_activeProcesses.wait_for(id, &child);
        }

        void controller::start_next_process(uint64_t oldStack) {
            scheduler::process* nextProcess = m_activeProcesses.get_next();

            if (nextProcess != nullptr) {
                if (nextProcess->execute_signals()) {
                    if (!nextProcess->state.is(scheduler::process_status::states::blocked)) {
                        if (nextProcess->is_static_scheduled()) {
                            module::timer::set_timer_sec(0.1);
                            module::timer::enable_timer();
                        } else {
                            module::timer::disable_timer();
                        }

                        nextProcess->resume(oldStack);
                    } else {
                        nextProcess->state.unset(scheduler::process_status::states::running);
                    }
                } else {
                    m_activeProcesses.remove_and_update(nextProcess);
                }
            } else {
                module::timer::set_timer_sec(1);
                module::timer::enable_timer();
                m_idleProcess->resume(oldStack);
            }
        }

        void controller::interrupt_process::handle(kernel::register_maps::general_registers& saved_registers) {
            // since we leave the exception-handle here, we need to close the active interrupt and restore the stackpointer
            // normally this would be performed after the handler-execution
            module::general_interrupt_controller::interface::cpu::ICC_EOIR1_EL1::store(controller::interrupt_process::id);
            context_switch(saved_registers);
        }

        scheduler::process* controller::get_current_process() {
            asm("mrs    x0, TPIDR_EL1");
        }

        void controller::set_current_process(scheduler::process* newProcess) {
            asm("msr    TPIDR_EL1, x0");
        }

        scheduler::process* controller::get_process(const uint64_t id) {
            return m_activeProcesses.search_for_id(id);
        }

        void controller::idle() {
            while (true) {
                asm("wfi");
            }
        }

        scheduler::process_list controller::m_activeProcesses;
        scheduler::process* controller::m_idleProcess;
        std::atomic<uint64_t> controller::m_nextProcessID;
    }
}
