#ifndef syscalls_h
#define syscalls_h

extern "C" void write(const char* string, const int size);

extern "C" int read(const char* string, const int size);

extern "C" void* malloc(const int size);

extern "C" void free(const void* ptr);

extern "C" int pid();

typedef uint64_t thread_t;

struct thread_options_t {
    uint64_t id;
    uint64_t asid;
    bool static_schedule;
    uint64_t stackSize;
};

extern "C" thread_t thread_create(void* arg, void (*func)(void*), thread_options_t* options);

extern "C" void thread_join(const thread_t t);

extern "C" void EXIT(int code);

extern "C" thread_t process_create(uint64_t address, uint64_t size, uint64_t entryPoint);

extern "C" uint64_t get_instr_retired();

#endif
