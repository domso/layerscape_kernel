#include "src/modules/scheduler/process.h"
#include "src/modules/mmu/controller.h"
#include "src/modules/scheduler/el0_start.h"

#include "src/modules/uart.h"

namespace module {
    namespace scheduler {
        void process::init(byte* codePA, const uint64_t codeLength, byte* entryAddress, const attributes& attr) {
            m_currentConfig = attr;
            m_map.init();
            m_mutex.init();

            m_saved_general_registers.clear();
            m_saved_fp_registers.clear();

            m_saved_system_registers.TPIDR_EL0 = m_currentConfig.id;
            m_saved_system_registers.TPIDRRO_EL0 = m_currentConfig.id;
            m_saved_system_registers.TPIDR_EL1 = (uint64_t)this;
            m_saved_system_registers.SPSR_EL1 = 0;

            m_selectedMutex = &m_mutex;

            init_code(codePA, codeLength, entryAddress);
            init_stack();

            parent = nullptr;
            waitfor = nullptr;
            no_context_store = false;
            state.clear();
        }

        void process::init_code(byte* codePA, const uint64_t codeLength, byte* entryAddress) {
            byte* pc;
            pc = m_map.simple_malloc(codeLength, module::mmu::table_entry::options::attr::ind3);

            uint64_t oldMap = module::mmu::controller::get_va_map_el0();
            module::mmu::controller::update_va_map_el0(m_map.get_basetable(), m_currentConfig.asid);

            for (int i = 0; i < codeLength; i++) {
                pc[i] = codePA[i];
            }

            module::mmu::controller::set_va_map_el0(oldMap);

            m_saved_system_registers.ELR_EL1 = pc + (entryAddress - codePA);
        }

        void process::init_stack() {
            byte* stack;
            stack = m_map.simple_malloc(m_currentConfig.stackSize, module::mmu::table_entry::options::attr::ind1);
            stack += m_currentConfig.stackSize;
            m_saved_system_registers.SP_EL0 = stack;
        }

        process* process::clone() {
            process* newProcess = (process*) kernel::system::page_alloc();

            if (newProcess != nullptr) {
                newProcess->m_currentConfig = m_currentConfig;
                newProcess->m_saved_general_registers = m_saved_general_registers;
                newProcess->m_saved_fp_registers = m_saved_fp_registers;
                newProcess->m_saved_system_registers = m_saved_system_registers;
                newProcess->m_map = m_map;
                newProcess->m_selectedMutex = &m_mutex;
                newProcess->state = state;
                newProcess->state.unset(process_status::states::running);
                newProcess->parent = this;
                newProcess->waitfor = waitfor;

                newProcess->init_stack();
            }

            return newProcess;
        }

        void process::resume(uint64_t oldStack) {
            module::mmu::controller::update_va_map_el0(m_map.get_basetable(), m_currentConfig.id);
            kernel::register_maps::fp_registers::load(&m_saved_fp_registers);

            m_saved_system_registers.TPIDR_EL1 = (uint64_t) this;
            m_saved_system_registers.TPIDR_EL0 = m_currentConfig.id;
            kernel::register_maps::system_registers::load(&m_saved_system_registers);

            module::scheduler::el0_start(m_saved_system_registers.ELR_EL1, m_saved_system_registers.SP_EL0, m_saved_system_registers.SPSR_EL1, oldStack, &m_saved_general_registers);
        }

        void process::store_context(kernel::register_maps::general_registers& saved_registers) {
            if (!no_context_store) {
                m_saved_general_registers = saved_registers;
                kernel::register_maps::fp_registers::store(&m_saved_fp_registers);
                kernel::register_maps::system_registers::store(&m_saved_system_registers);
            }
        }

        bool process::is_static_scheduled() const {
            return m_currentConfig.static_schedule;
        }

        process::attributes& process::config() {
            return m_currentConfig;
        }

        byte* process::malloc(const uint64_t size, const uint64_t attrIndx) {
            byte* result;

            m_selectedMutex->lock();
            result = m_map.simple_malloc(size, attrIndx);
            m_selectedMutex->unlock();

            return result;
        }

        void process::free(const byte* start) {
            m_selectedMutex->lock();
            m_map.simple_free(start, config().asid);
            m_selectedMutex->unlock();
        }

        void process::setPC(byte* addr) {
            m_saved_system_registers.ELR_EL1 = addr;
        }

        void process::signal(const uint64_t signal) {
            state.set(signal);
        }
 
        bool process::execute_signals() {
            bool continueProcess = true;
            uint64_t signals = state.get();

            if (signals & process_status::signals::SIGINT) {
                continueProcess = false;
                destroy();
            }

            if (signals & process_status::signals::SIGKILL) {
                continueProcess = false;
                destroy();
            }

            if (signals & process_status::signals::SIGTERM) {
                continueProcess = false;
                destroy();
            }

            if (signals & process_status::signals::SIGSTOP) {
                state.set(process_status::states::blocked);
            }

            if (signals & process_status::signals::SIGCONT) {
                state.unset(process_status::states::blocked);
            }

            state.unset(process_status::signals::SIGANY);

            return continueProcess;
        }

        void process::destroy() {
            if (m_currentConfig.asid == m_currentConfig.id) {
                m_selectedMutex->lock();
                m_map.clear();
                m_selectedMutex->unlock();
            }
        }
    }
}
