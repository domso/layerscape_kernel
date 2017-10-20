#ifndef apps_h
#define apps_h

#include <stdint.h>
#include "string.h"
#include "syscalls.h"

void app_list();

struct app {    
    static void init();
    
    void operator =(const app& a);
    
    template <typename T>
    static void register_app() {
        if (currentAppSize <= currentAppNum) {
            app* newAppVector = (app*) malloc(currentAppSize * 2 * sizeof(app));
            
            for (int i = 0; i < currentAppNum; i++) {
                newAppVector[i] = appVector[i];
            }
            
            currentAppSize *= 2;
            free(appVector);
            appVector = newAppVector;
        }
        
        appVector[currentAppNum].address = T::address;
        appVector[currentAppNum].entryAddress = T::entryAddress;
        appVector[currentAppNum].size = T::size;
        appVector[currentAppNum].command = (char*)T::command;    
        currentAppNum++;
    }

    static bool start_exec(string& cmd);
    
    uint64_t address;
    uint64_t size;
    uint64_t entryAddress;
    char* command;
    
    static app* appVector;
    static int currentAppSize;
    static int currentAppNum;
};

#endif