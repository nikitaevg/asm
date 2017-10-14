#include "allocator.hpp"
#include <sys/mman.h>

void *allocator::allocate()
{
    void *ptr = allocated;
    allocated = static_cast<void **>(*allocated);
    return ptr;
}

void allocator::deallocate(void *ptr)
{
    *static_cast<void **>(ptr) = allocated;
    allocated = static_cast<void **>(ptr);
}

allocator::allocator()
{
    first_alloc = allocated = static_cast<void **>(mmap(0, PAGE_SIZE, PROT_EXEC | PROT_READ | PROT_WRITE,
                                                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
    if (allocated == MAP_FAILED)
        throw MAP_FAILED;
    for (char *ch = reinterpret_cast<char *>(allocated) + TRAMPOLINE_SIZE;
         ch < reinterpret_cast<char *>(allocated) + PAGE_SIZE; ch += TRAMPOLINE_SIZE) {
        *reinterpret_cast<void **>(ch - TRAMPOLINE_SIZE) = (ch);
    }
}

allocator::~allocator()
{
    munmap(first_alloc, PAGE_SIZE);
}
