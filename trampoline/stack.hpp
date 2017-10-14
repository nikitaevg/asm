#ifndef TRAMPOLINE_STACK_HPP
#define TRAMPOLINE_STACK_HPP

#include <string>

struct stack {
    explicit stack(void *top);

    void push(const std::string &inst);

    void push(const std::string &inst, size_t);

    void push(const std::string &inst, void *obj);

    char *get_ip();

    void *jmp(size_t offset);

private:
    char *code;

};

const std::string
        mov_rsi_rdi = "\x48\x89\xFE",
        mov_rdx_rsi = "\x48\x89\xF2",
        mov_rcx_rdx = "\x48\x89\xD1",
        mov_r8_rcx = "\x49\x89\xC8",
        mov_r9_r8 = "\x4D\x89\xC1",
        mov_rdi = "\x48\xBF",
        mov_rax = "\x48\xB8",
        jmp_rax = "\xFF\xE0",
        mov_r11_rsp = "\x4C\x8B\x1C\x24",
        push_r9 = "\x41\x51",
        mov_rax_rsp = "\x48\x89\xE0",
        add_rax = "\x48\x05",
        add_rsp = "\x48\x81\xC4",
        cmp_rax_rsp = "\x48\x39\xE0",
        je = "\x74",
        mov_rdi_rsp = "\x48\x8B\x3C\x24",
        mov_sub_rsp_8_rdi = "\x48\x89\x7C\x24\xF8",
        jmp = "\xEB",
        mov_rsp_r11 = "\x4C\x89\x1C\x24",
        sub_rsp = "\x48\x81\xEC",
        call_rax = "\xFF\xD0",
        pop_r9 = "\x41\x59",
        mov_r11_add_rsp = "\x4C\x8B\x9C\x24",
        ret = "\xC3";


#endif //TRAMPOLINE_STACK_HPP
