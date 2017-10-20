#include "src/modules/scheduler/process_list.h"

#include "src/modules/uart.h"

namespace module {
    namespace scheduler {
        void process_list::init() {
            head = nullptr;
            tail = nullptr;
            mutex.init();
        }

        scheduler::process* process_list::get_next() {
            scheduler::process* result = nullptr;
            scheduler::process* first = nullptr;

            mutex.lock();
            result = head;
            mutex.unlock();

            if (result != nullptr) {
                mutex.lock();

                do {
                    result = internal_pop();
                    internal_push(result);

                    if (first == nullptr) {
                        first = result;
                    } else if (first == result) {
                        result = nullptr;
                        break;
                    }

                } while (result->state.is(process_status::states::running));

                mutex.unlock();
            }

            if (result != nullptr) {
                result->state.set(process_status::states::running);
            }

            return result;
        }

        scheduler::process* process_list::internal_pop() {
            scheduler::process* result = head;

            if (result != nullptr) {
                head = head->next;
            }

            return result;
        }

        void process_list::push(scheduler::process* process) {
            mutex.lock();
            internal_push(process);
            mutex.unlock();

            asm("sev");
        }

        void process_list::internal_push(scheduler::process* process) {
            process->next = nullptr;

            if (head == nullptr) {
                head = process;
                tail = process;
            } else {
                tail->next = process;
                tail = process;
            }
        }

        void process_list::remove_and_update(module::scheduler::process* process) {
            scheduler::process* old = nullptr;

            mutex.lock();

            for (auto current = head; current != nullptr; current = current->next) {
                if (current == process) {
                    if (old == nullptr) {
                        head = current->next;
                    } else {
                        old->next = current->next;

                        if (tail == current) {
                            tail = old;
                        }
                    }
                }

                if (current->parent == process) {
                    current->signal(process_status::signals::SIGTERM);
                }

                if (current->waitfor == process) {
                    current->waitfor = nullptr;
                    current->signal(process_status::signals::SIGCONT);
                }

                old = current;
            }

            mutex.unlock();
        }

        bool process_list::wait_for(const uint64_t id, scheduler::process* process) {
            bool result = false;

            mutex.lock();

            for (auto current = head; current != nullptr; current = current->next) {
                if (current->config().id == id) {
                    process->waitfor = current;
                    process->signal(process_status::signals::SIGSTOP);
                    result = true;
                    break;
                }
            }

            mutex.unlock();
            return result;
        }

        scheduler::process* process_list::search_for_id(const uint64_t id) {
            scheduler::process* result = nullptr;

            mutex.lock();

            for (auto current = head; current != nullptr; current = current->next) {
                if (current->config().id == id) {
                    result = current;
                    break;
                }
            }

            mutex.unlock();

            return result;
        }

        void process_list::print() {
            mutex.lock();

            for (auto current = head; current != nullptr; current = current->next) {
                module::uart::print("ID: ");
                module::uart::printHex(current->config().id);
                module::uart::print(" ASID: ");
                module::uart::printHex(current->config().asid);
                module::uart::print(" STATUS: ");
                module::uart::printHex(current->state.get());
                module::uart::println("");
            }

            mutex.unlock();
        }
    }
}
