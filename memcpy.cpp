#include <stddef.h>
#include <emmintrin.h>
#include <iostream>
#include <cstring>
#include <assert.h>

void *memcpy128(void *destination, const void *source, size_t num)
{
    size_t const step = sizeof(__m128i);
    void *old_dst = destination;
    char *dst = static_cast<char *>(destination);
    const char *src = static_cast<const char *>(source);
    auto stupid_memcpy = [](auto &&pred, char *&_destination, const char *&_source, size_t &_num)
    {
        while (pred(_destination) && _num)
        {
            *(_destination++) = *(_source++);
            _num--;
        }
    };
    stupid_memcpy([step](auto x){return reinterpret_cast<size_t>(x) % step != 0;}, dst, src, num);

    while (num >= step) {
        __m128i tmp;
        asm volatile (
            "movdqu     (%1), %0\n"
            "movntdq    %0, (%2)\n"
        : "=x"(tmp)
        : "r"(src), "r"(dst)
        );
        num -= step;
        dst += step;
        src += step;
    }

    stupid_memcpy([](auto x){return true;}, dst, src, num);
    return old_dst;
}

int main(void)
{
    char x[] = "abacabadabacabaeabacabadabacabafabacabadabacabaeabacabadabacaba", *y = new char[sizeof(x) + 10];
    memcpy128(y, x, sizeof(x));
    memcpy128(y + 1, x, sizeof(x));
    memcpy128(y + 3, x, sizeof(x));
    assert (strncmp(y + 3, x, sizeof(x)) == 0);
    std::cout << y + 2;
}
