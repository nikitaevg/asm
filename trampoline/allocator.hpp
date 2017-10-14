#ifndef TRAMPOLINE_ALLOCATOR_HPP
#define TRAMPOLINE_ALLOCATOR_HPP


#include <cstddef>

struct allocator {
    allocator();
    ~allocator();

    void *allocate();
    void deallocate(void *ptr);
private:
    void **allocated;
    void **first_alloc;
    static constexpr size_t PAGE_SIZE = 1 << 12;
    static constexpr size_t TRAMPOLINE_SIZE = 1 << 8;
};


#endif //TRAMPOLINE_ALLOCATOR_HPP
