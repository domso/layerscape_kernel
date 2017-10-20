#include "src/string.h"

string::string(const char* str, const uint64_t l) : c_str(str), length(l) {

}

string::string(const char* str) : c_str(str) {
    for (length = 0; str[length] != '\0'; length++) {}
}

string::string() : c_str("\0"), length(1) {}

bool string::operator==(const string& other) const{
    bool result = true;
    if (other.length == length) {
        for (int i = 0; i < length; i++) {
            if (c_str[i] != other.c_str[i]) {
                result = false;
                break;
            }
        }
    } else {
        result = false;
    }
    
    return result;
}