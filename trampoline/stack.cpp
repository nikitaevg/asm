#include "stack.hpp"

stack::stack(void *top) : code(static_cast<char *>(top))
{

}

void stack::push(const std::string &inst)
{
    for (char ch : inst)
        *(code++) = ch;
}

void stack::push(const std::string &inst, size_t arg1)
{
    push(inst);
    *(size_t *)code = arg1;
    code += 4;
}

void stack::push(const std::string &inst, void *obj)
{
    push(inst);
    *(void **) code = obj;
    code += 8;
}

char *stack::get_ip()
{
    return code;
}

void * stack::jmp(size_t offset)
{
    void *prev = code;
    code += offset;
    return prev;
}
