#include "src/modules/performance_counter.h"

namespace module {
    bool performance_counter::load() {
        asm(
            "mov    x1, #0x1\n"
            "msr    PMCR_EL0, x1"
        );

        return true;
    }

    bool performance_counter::event_available(const uint32_t event) {
        uint64_t mask = event;
        uint32_t pmce = 0;
        mask = 1UL << mask;

        if (mask & 0xFFFFFFFF) {
            asm(
                "mrs    %[pmce], PMCEID0_EL0\n"
                : [pmce] "=r"(pmce)
            );
        } else {
            asm(
                "mrs    %[pmce], PMCEID1_EL0\n"
                : [pmce] "=r"(pmce)
            );
            mask = mask >> 32;
        }

        return (pmce & mask) != 0;
    }

    uint32_t performance_counter::cycle_counter::read() {
        asm("mrs    x0, PMCCNTR_EL0\n");
    }

    void performance_counter::cycle_counter::clear() {
        asm("msr    PMCCNTR_EL0, xzr");

    }

    void performance_counter::port0::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER0_EL0, x0"
        );
    }

    uint32_t performance_counter::port0::read() {
        asm("mrs    x0, PMEVCNTR0_EL0\n");
    }

    void performance_counter::port0::clear() {
        asm("msr    PMEVCNTR0_EL0, xzr");
    }

    void performance_counter::port1::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER1_EL0, x0"
        );
    }

    uint32_t performance_counter::port1::read() {
        asm("mrs    x0, PMEVCNTR1_EL0\n");
    }

    void performance_counter::port1::clear() {
        asm("msr    PMEVCNTR1_EL0, xzr");
    }

    void performance_counter::port2::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER2_EL0, x0"
        );
    }

    uint32_t performance_counter::port2::read() {
        asm("mrs    x0, PMEVCNTR2_EL0\n");
    }

    void performance_counter::port2::clear() {
        asm("msr    PMEVCNTR2_EL0, xzr");
    }

    void performance_counter::port3::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER3_EL0, x0"
        );
    }

    uint32_t performance_counter::port3::read() {
        asm("mrs    x0, PMEVCNTR3_EL0\n");
    }

    void performance_counter::port3::clear() {
        asm("msr    PMEVCNTR3_EL0, xzr");
    }

    void performance_counter::port4::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER4_EL0, x0"
        );
    }

    uint32_t performance_counter::port4::read() {
        asm("mrs    x0, PMEVCNTR4_EL0\n");
    }

    void performance_counter::port4::clear() {
        asm("msr    PMEVCNTR4_EL0, xzr");
    }

    void performance_counter::port5::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER5_EL0, x0"
        );
    }

    uint32_t performance_counter::port5::read() {
        asm("mrs    x0, PMEVCNTR5_EL0\n");
    }

    void performance_counter::port5::clear() {
        asm("msr    PMEVCNTR5_EL0, xzr");
    }

    void performance_counter::port6::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER6_EL0, x0"
        );
    }

    uint32_t performance_counter::port6::read() {
        asm("mrs    x0, PMEVCNTR6_EL0\n");
    }

    void performance_counter::port6::clear() {
        asm("msr    PMEVCNTR6_EL0, xzr");
    }

    void performance_counter::port7::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER7_EL0, x0"
        );
    }

    uint32_t performance_counter::port7::read() {
        asm("mrs    x0, PMEVCNTR7_EL0\n");
    }

    void performance_counter::port7::clear() {
        asm("msr    PMEVCNTR7_EL0, xzr");
    }

    void performance_counter::port8::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER8_EL0, x0"
        );
    }

    uint32_t performance_counter::port8::read() {
        asm("mrs    x0, PMEVCNTR8_EL0\n");
    }

    void performance_counter::port8::clear() {
        asm("msr    PMEVCNTR8_EL0, xzr");
    }

    void performance_counter::port9::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER9_EL0, x0"
        );
    }

    uint32_t performance_counter::port9::read() {
        asm("mrs    x0, PMEVCNTR9_EL0\n");
    }

    void performance_counter::port9::clear() {
        asm("msr    PMEVCNTR9_EL0, xzr");
    }

    void performance_counter::port10::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER10_EL0, x0"
        );
    }

    uint32_t performance_counter::port10::read() {
        asm("mrs    x0, PMEVCNTR10_EL0\n");
    }

    void performance_counter::port10::clear() {
        asm("msr    PMEVCNTR10_EL0, xzr");
    }

    void performance_counter::port11::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER11_EL0, x0"
        );
    }

    uint32_t performance_counter::port11::read() {
        asm("mrs    x0, PMEVCNTR11_EL0\n");
    }

    void performance_counter::port11::clear() {
        asm("msr    PMEVCNTR11_EL0, xzr");
    }

    void performance_counter::port12::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER12_EL0, x0"
        );
    }

    uint32_t performance_counter::port12::read() {
        asm("mrs    x0, PMEVCNTR12_EL0\n");
    }

    void performance_counter::port12::clear() {
        asm("msr    PMEVCNTR12_EL0, xzr");
    }

    void performance_counter::port13::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER13_EL0, x0"
        );
    }

    uint32_t performance_counter::port13::read() {
        asm("mrs    x0, PMEVCNTR13_EL0\n");
    }

    void performance_counter::port13::clear() {
        asm("msr    PMEVCNTR13_EL0, xzr");
    }

    void performance_counter::port14::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER14_EL0, x0"
        );
    }

    uint32_t performance_counter::port14::read() {
        asm("mrs    x0, PMEVCNTR14_EL0\n");
    }

    void performance_counter::port14::clear() {
        asm("msr    PMEVCNTR14_EL0, xzr");
    }

    void performance_counter::port15::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER15_EL0, x0"
        );
    }

    uint32_t performance_counter::port15::read() {
        asm("mrs    x0, PMEVCNTR15_EL0\n");
    }

    void performance_counter::port15::clear() {
        asm("msr    PMEVCNTR15_EL0, xzr");
    }

    void performance_counter::port16::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER16_EL0, x0"
        );
    }

    uint32_t performance_counter::port16::read() {
        asm("mrs    x0, PMEVCNTR16_EL0\n");
    }

    void performance_counter::port16::clear() {
        asm("msr    PMEVCNTR16_EL0, xzr");
    }

    void performance_counter::port17::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER17_EL0, x0"
        );
    }

    uint32_t performance_counter::port17::read() {
        asm("mrs    x0, PMEVCNTR17_EL0\n");
    }

    void performance_counter::port17::clear() {
        asm("msr    PMEVCNTR17_EL0, xzr");
    }

    void performance_counter::port18::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER18_EL0, x0"
        );
    }

    uint32_t performance_counter::port18::read() {
        asm("mrs    x0, PMEVCNTR18_EL0\n");
    }

    void performance_counter::port18::clear() {
        asm("msr    PMEVCNTR18_EL0, xzr");
    }

    void performance_counter::port19::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER19_EL0, x0"
        );
    }

    uint32_t performance_counter::port19::read() {
        asm("mrs    x0, PMEVCNTR19_EL0\n");
    }

    void performance_counter::port19::clear() {
        asm("msr    PMEVCNTR19_EL0, xzr");
    }

    void performance_counter::port20::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER20_EL0, x0"
        );
    }

    uint32_t performance_counter::port20::read() {
        asm("mrs    x0, PMEVCNTR20_EL0\n");
    }

    void performance_counter::port20::clear() {
        asm("msr    PMEVCNTR20_EL0, xzr");
    }

    void performance_counter::port21::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER21_EL0, x0"
        );
    }

    uint32_t performance_counter::port21::read() {
        asm("mrs    x0, PMEVCNTR21_EL0\n");
    }

    void performance_counter::port21::clear() {
        asm("msr    PMEVCNTR21_EL0, xzr");
    }

    void performance_counter::port22::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER22_EL0, x0"
        );
    }

    uint32_t performance_counter::port22::read() {
        asm("mrs    x0, PMEVCNTR22_EL0\n");
    }

    void performance_counter::port22::clear() {
        asm("msr    PMEVCNTR22_EL0, xzr");
    }

    void performance_counter::port23::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER23_EL0, x0"
        );
    }

    uint32_t performance_counter::port23::read() {
        asm("mrs    x0, PMEVCNTR23_EL0\n");
    }

    void performance_counter::port23::clear() {
        asm("msr    PMEVCNTR23_EL0, xzr");
    }

    void performance_counter::port24::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER24_EL0, x0"
        );
    }

    uint32_t performance_counter::port24::read() {
        asm("mrs    x0, PMEVCNTR24_EL0\n");
    }

    void performance_counter::port24::clear() {
        asm("msr    PMEVCNTR24_EL0, xzr");
    }

    void performance_counter::port25::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER25_EL0, x0"
        );
    }

    uint32_t performance_counter::port25::read() {
        asm("mrs    x0, PMEVCNTR25_EL0\n");
    }

    void performance_counter::port25::clear() {
        asm("msr    PMEVCNTR25_EL0, xzr");
    }

    void performance_counter::port26::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER26_EL0, x0"
        );
    }

    uint32_t performance_counter::port26::read() {
        asm("mrs    x0, PMEVCNTR26_EL0\n");
    }

    void performance_counter::port26::clear() {
        asm("msr    PMEVCNTR26_EL0, xzr");
    }

    void performance_counter::port27::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER27_EL0, x0"
        );
    }

    uint32_t performance_counter::port27::read() {
        asm("mrs    x0, PMEVCNTR27_EL0\n");
    }

    void performance_counter::port27::clear() {
        asm("msr    PMEVCNTR27_EL0, xzr");
    }

    void performance_counter::port28::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER28_EL0, x0"
        );
    }

    uint32_t performance_counter::port28::read() {
        asm("mrs    x0, PMEVCNTR28_EL0\n");
    }

    void performance_counter::port28::clear() {
        asm("msr    PMEVCNTR28_EL0, xzr");
    }

    void performance_counter::port29::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER29_EL0, x0"
        );
    }

    uint32_t performance_counter::port29::read() {
        asm("mrs    x0, PMEVCNTR29_EL0\n");
    }

    void performance_counter::port29::clear() {
        asm("msr    PMEVCNTR29_EL0, xzr");
    }

    void performance_counter::port30::listen(const uint32_t event, const uint32_t options) {
        asm(
            "orr    x0, x0, x1\n"
            "msr    PMEVTYPER30_EL0, x0"
        );
    }

    uint32_t performance_counter::port30::read() {
        asm("mrs    x0, PMEVCNTR30_EL0\n");
    }

    void performance_counter::port30::clear() {
        asm("msr    PMEVCNTR30_EL0, xzr");
    }
}
