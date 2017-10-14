#ifndef TRAMPOLINE_TRAMPOLINE_HPP
#define TRAMPOLINE_TRAMPOLINE_HPP

#include "allocator.hpp"
#include "stack.hpp"
#include "args_count.h"

const std::string shift_registers[5] = {
        mov_rsi_rdi,
        mov_rdx_rsi,
        mov_rcx_rdx,
        mov_r8_rcx,
        mov_r9_r8
};

template<typename T>
struct trampoline;

template<typename Ret, typename ...Args>
struct trampoline<Ret(Args ...)> {
    template<typename F>
    trampoline(const F &func) : function(new F(func)), caller(&call_impl < F > ), deleter(&delete_impl < F > )
    {
        pstack = alloc.allocate();
        auto ints = args_count<Args...>::INT, sses = args_count<Args...>::SSE;
        stack stack_impl{pstack};
        if (ints < 6) {
            for (ssize_t i = ints - 1; i >= 0; i--) {
                stack_impl.push(shift_registers[i]);
            }
            stack_impl.push(mov_rdi, function);
            stack_impl.push(mov_rax, (void *) caller);
            stack_impl.push(jmp_rax);
        }
            //RDI, RSI, RDX, RCX, R8, R9
        else {
            if (sses > 8)
                sses -= 8;
            else
                sses = 0;
            auto stack_size = (ints - 6 + sses) * 8;
            stack_impl.push(mov_r11_rsp); // save ret address
            stack_impl.push(push_r9);
            for (ssize_t i = 4; i >= 0; i--) {
                stack_impl.push(shift_registers[i]);
            }
            stack_impl.push(mov_rax_rsp);
            stack_impl.push(add_rax, stack_size + 8);
            stack_impl.push(add_rsp, 8);

            // shift stack arguments
            char *loop = stack_impl.get_ip();
            stack_impl.push(cmp_rax_rsp);
            stack_impl.push(je);
            char *cont = stack_impl.get_ip();
            stack_impl.jmp(1);
            stack_impl.push(add_rsp, 8);
            stack_impl.push(mov_rdi_rsp);
            stack_impl.push(mov_sub_rsp_8_rdi);
            stack_impl.push(jmp);

            char *curr = stack_impl.get_ip();
            *curr = static_cast<char>(loop - stack_impl.get_ip()); // put label for loop
            stack_impl.jmp(1);
            *cont = static_cast<char>(stack_impl.get_ip() - cont - 1); //put label

            // prepare for calling
            stack_impl.push(mov_rsp_r11);
            stack_impl.push(sub_rsp, stack_size + 8);
            stack_impl.push(mov_rdi, function);
            stack_impl.push(mov_rax, (void *) caller);

            stack_impl.push(call_rax); // call function

            // restore stack
            stack_impl.push(pop_r9);
            stack_impl.push(mov_r11_add_rsp, stack_size);
            stack_impl.push(mov_rsp_r11);
            stack_impl.push(ret);
        }
    }

    trampoline(const trampoline &rhs) = delete;

    trampoline(trampoline &&rhs) : function(rhs.function), pstack(rhs.pstack),
                                   caller(rhs.caller), deleter(rhs.deleter)
    {
        rhs.deleter = nullptr;
        rhs.caller = nullptr;
        rhs.function = rhs.pstack = nullptr;
    }

    ~trampoline()
    {
        if (function) {
            deleter(function);
            alloc.deallocate(pstack);
        }
    }

    trampoline &operator=(trampoline &&other)
    {
        function = other.function;
        pstack = other.pstack;
        caller = other.caller;
        deleter = other.deleter;

        other.function = other.pstack = nullptr;
        other.caller = nullptr;
        other.deleter = nullptr;
    }

    Ret (*get())(Args... args)
    {
        return (Ret (*)(Args...)) pstack;
    }

private:
    template<typename F>
    static Ret call_impl(void *_this, Args... args)
    {
        return (*(F *) _this)(args ...);
    }

    template<typename F>
    static void delete_impl(void *function)
    {
        delete static_cast<F *>(function);
    }

    static allocator alloc;
    void *function;
    void *pstack;

    Ret (*caller)(void *obj, Args... args);

    void (*deleter)(void *obj);
};

template<typename R, typename ... Args>
allocator trampoline<R(Args...)>::alloc;

#endif //TRAMPOLINE_TRAMPOLINE_HPP
