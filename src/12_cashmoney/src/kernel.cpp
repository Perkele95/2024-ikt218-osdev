extern "C" int kernel_main(void);

#include <libc/memory.h>

void *operator new(size_t size) {
    return malloc(size);
}

void *operator new[](size_t size) {
    return malloc(size);
}

void operator delete(void *ptr) noexcept {
    free(ptr);
}

void operator delete[](void *ptr) noexcept {
    free(ptr);
}

void operator delete(void *ptr, size_t size) noexcept {
    (void)size; // Size parameter is unused, added to match required signature
    free(ptr);
}

void operator delete[](void *ptr, size_t size) noexcept {
    (void)size; // Size parameter is unused, added to match required signature
    free(ptr);
}

int kernel_main()
{
    auto test = new char[10];
    delete[] test;

    while(true){}
    return 0;
}