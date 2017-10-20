#ifndef string_h
#define string_h

#include <stdint.h>

struct string {
    string(const char* str, const uint64_t l);

    string(const char* str);
    
    string();

    bool operator==(const string& other) const;

    const char * c_str;
    uint64_t length;
};

#endif