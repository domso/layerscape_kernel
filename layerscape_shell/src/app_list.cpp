#include "src/apps.h"

struct helloworld {
    constexpr static const uint64_t address      = 0xb1000000;
    constexpr static const uint64_t entryAddress = 0xb10000a8;
    constexpr static const uint64_t size         = 4096;
    constexpr static const char*    command      = "Hello";
};

struct test {
    constexpr static const uint64_t address      = 0xb1100000;
    constexpr static const uint64_t entryAddress = 0xb11000f5;
    constexpr static const uint64_t size         = 4096;
    constexpr static const char*    command      = "test";
};


void app_list() {
    app::register_app<helloworld>();
    app::register_app<test>();
}