#include "src/modules/mmu/table_entry.h"

namespace module {
    namespace mmu {
        void table_entry::set(const uint64_t mask, const uint64_t options) {
            m_fields = (m_fields & (~mask)) | (options & mask);
        }

        void table_entry::enable(const uint64_t mask) {
            m_fields = m_fields | mask;
        }

        void table_entry::disable(const uint64_t mask) {
            m_fields = m_fields & (~mask);
        }

        bool table_entry::check(const uint64_t mask, const uint64_t options) {
            return (m_fields & mask) == options;
        }

        uint64_t table_entry::get(const uint64_t mask) {
            return m_fields & mask;
        }

        void table_entry::clear() {
            m_fields = 0;
        }
    }
}
