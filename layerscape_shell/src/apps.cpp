#include "src/apps.h"
#include "src/syscalls.h"

void app::init() {
    currentAppNum = 0;
    
    currentAppSize = 4096;
    appVector = (app*) malloc(4096 * sizeof(app));    
}

void app::operator=(const app& a) {
    address = a.address;
    command = a.command;
}

bool app::start_exec(string& cmd) {
    bool result = false;
    for (int i = 0; i < currentAppNum; i++) {
        string command(appVector[i].command);
        if (command == cmd) {
            thread_t proc = process_create(appVector[i].address, appVector[i].size, appVector[i].entryAddress);
            thread_join(proc);
            result = true;
            break;
        }
    }
    return result;
} 

app* app::appVector;
int app::currentAppSize;
int app::currentAppNum;