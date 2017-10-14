#ifndef TRAMPOLINE_ARGS_COUNT_H
#define TRAMPOLINE_ARGS_COUNT_H

#include <cstdio>
#include <mmintrin.h>

template <typename ...Args>
struct args_count;

template <>
struct args_count<>
{
    static constexpr size_t INT = 0;
    static constexpr size_t SSE = 0;
};

template <typename ...Args>
struct args_count<double, Args...>
{
    static constexpr size_t INT = args_count<Args...>::INT;
    static constexpr size_t SSE = args_count<Args...>::SSE + 1;
};

template <typename ...Args>
struct args_count<float, Args...>
{
    static constexpr size_t INT = args_count<Args...>::INT;
    static constexpr size_t SSE = args_count<Args...>::SSE + 1;
};

template <typename ...Args>
struct args_count<__m64, Args...>
{
    static constexpr size_t INT = args_count<Args...>::INT;
    static constexpr size_t SSE = args_count<Args...>::SSE + 1;
};

template <typename T, typename ...Args>
struct args_count<T, Args...>
{
    static constexpr size_t INT = args_count<Args...>::INT + 1;
    static constexpr size_t SSE = args_count<Args...>::SSE;
};

#endif //TRAMPOLINE_ARGS_COUNT_H
